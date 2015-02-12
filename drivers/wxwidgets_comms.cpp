// Copyright (C) 2008  Werner Smekal
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//

#include "wxwidgets_comms.h"
#include <assert.h>

//--------------------------------------------------------------------------
//  Constructor, creates the object but does not actually create or link to
//  any shared memory.
//--------------------------------------------------------------------------
PLMemoryMap::PLMemoryMap()
{
#ifdef WIN32
	m_mapFile=NULL;
#else
	m_mapFile = -1;
	m_name = NULL;
#endif
	m_buffer=NULL;
	m_size = 0;
}

//--------------------------------------------------------------------------
//  Constructor, creates the shared memory area. If onlyIfExists is true
//  then we will try to access an existing shared memory area rather than
//  creating a new one.
//--------------------------------------------------------------------------
PLMemoryMap::PLMemoryMap( const char *name, PLINT size, bool mustExist, bool mustNotExist )
{
#ifdef WIN32
	m_mapFile = NULL;
#else
	m_mapFile = -1;
	m_name = NULL;
#endif
	m_buffer=NULL;
	m_size = 0;
	create( name, size, mustExist, mustNotExist );
}

//--------------------------------------------------------------------------
//  create does most of the work in trying to create the memory map it is
//  called by the constructor or by the user. If the object already has a
//  shared memory area then that is closed before a new area of memory is
//  created or connected to. As per the constructor if onlyIfExists is true
//  then we will try to access an existing shared memory area rather than
//  creating a new one.
//--------------------------------------------------------------------------
void PLMemoryMap::create( const char *name, PLINT size, bool mustExist, bool mustNotExist )
{
	close();
	assert( ! (mustExist && mustNotExist ) );
	if( mustExist && mustNotExist )
		return;
#ifdef WIN32
	if( mustExist )
		m_mapFile = OpenFileMappingA( FILE_MAP_ALL_ACCESS, FALSE, name);
	else if( mustNotExist )
	{
		m_mapFile = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL,
			PAGE_READWRITE, 0, size, name );
		if( GetLastError() == ERROR_ALREADY_EXISTS )
			close();
	}
	else
		m_mapFile = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL,
			PAGE_READWRITE, 0, size, name );

	if( m_mapFile )
		m_buffer = MapViewOfFile( m_mapFile, FILE_MAP_ALL_ACCESS, 0, 0, size );
#else
	if( mustExist )
		m_mapFile = shm_open( name, O_RDWR, 0 );
	else if( mustNotExist )
	{
		m_mapFile = shm_open( name, O_RDWR|O_CREAT|O_EXCL, S_IRWXU ); //S_IRWXU gives user wrx permissions
		if( ftruncate( m_mapFile, size ) == -1 )
			close( );
	}
	else
	{
		m_mapFile = shm_open( name, O_RDWR|O_CREAT, S_IRWXU ); //S_IRWXU gives user wrx permissions
		if( ftruncate( m_mapFile, size ) == -1 )
			close( );
	}
	if( m_mapFile != -1 )
	{
		m_buffer = mmap( NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, m_mapFile, 0 );
		m_name = new char[strlen( name ) + 1];
		strcpy( m_name, name );
	}
#endif
	if( isValid() )
		m_size = size;
}

//--------------------------------------------------------------------------
//  Close an area of mapped memory. When all processes have closed their
//  connections the area will be removed by the OS.
//--------------------------------------------------------------------------
void PLMemoryMap::close()
{
#ifdef WIN32
	if( m_buffer )
		UnmapViewOfFile( m_buffer );
	if( m_mapFile )
		CloseHandle( m_mapFile );
	m_mapFile = NULL;
#else
	if( m_buffer )
		munmap( m_buffer, m_size );
	if( m_mapFile != -1 )
		shm_unlink( m_name );
	if( m_name )
		delete[] m_name;
	m_mapFile = -1;
	m_name = NULL;
#endif
	m_buffer = NULL;
	m_size = 0;
}

//--------------------------------------------------------------------------
//  Destructor, closes the connection to the mapped memory.
//--------------------------------------------------------------------------
PLMemoryMap::~PLMemoryMap()
{
	close();
}


PLNamedMutex::PLNamedMutex()
{
	m_mutex = NULL;
	m_haveLock = false;
}

PLNamedMutex::PLNamedMutex( const char *name, bool aquireOnCreate )
{
	m_mutex = NULL;
	m_haveLock = false;
	create( name, aquireOnCreate );
}

void PLNamedMutex::create( const char *name, bool aquireOnCreate )
{
#ifdef WIN32
	m_mutex = CreateMutexA( NULL, aquireOnCreate ? TRUE : FALSE, name );
#else
	m_mutex = NULL;
	char mutexName[251];
	mutexName[0] = '/';
	strncpy( mutexName + 1, name, 250 );
	mutexName[250] = '\0';
	m_mutex = sem_open( mutexName, O_CREAT, S_IRWXU, 1 );
#endif
}

void PLNamedMutex::aquire()
{
#ifdef WIN32
	DWORD result = WaitForSingleObject( m_mutex, INFINITE );
	m_haveLock = ( result == WAIT_OBJECT_0 || result == WAIT_ABANDONED );
#else
	m_haveLock = sem_wait( m_mutex ) == 0;
	int result = errno;
#endif
	if( !m_haveLock )
		throw( result );
}

bool PLNamedMutex::aquire( unsigned long millisecs)
{
#ifdef WIN32
	DWORD result = WaitForSingleObject( m_mutex, millisecs );
	m_haveLock = ( result == WAIT_OBJECT_0 || result == WAIT_ABANDONED );
#else
#endif
	return m_haveLock;
}

bool PLNamedMutex::aquireNoWait()
{
#ifdef WIN32
	m_haveLock = ( WAIT_OBJECT_0 == WaitForSingleObject( m_mutex, 0 ) );
#else
	m_haveLock = sem_trywait( m_mutex ) == 0;
#endif
	return m_haveLock;
}

void PLNamedMutex::release()
{
	if( !m_haveLock )
		return;
#ifdef WIN32
	if( m_mutex )
		ReleaseMutex( m_mutex );
#else
	sem_post ( m_mutex );
#endif
	m_haveLock = false;
}

void PLNamedMutex::clear()
{
	release();
#ifdef WIN32
	CloseHandle( m_mutex );
#else
	sem_close( m_mutex );
#endif
}

PLNamedMutex::~PLNamedMutex()
{
	clear();
}

PLNamedMutexLocker::PLNamedMutexLocker( PLNamedMutex *mutex )
{
	m_mutex = mutex;
	m_mutex->aquire();
}

bool PLNamedMutex::isValid()
{
	return m_mutex != NULL;
}

PLNamedMutexLocker::~PLNamedMutexLocker( )
{
	m_mutex ->release();
	m_mutex = NULL;
}