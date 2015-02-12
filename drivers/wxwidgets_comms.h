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

#ifndef __PL_WXWIDGETS_COMMS__
#define __PL_WXWIDGETS_COMMS__

#include "plplotP.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#endif

//data transmission codes
const unsigned char transmissionRegular = 0;
const unsigned char transmissionSkipFileEnd = 1;
const unsigned char transmissionEndOfPage = 2;
const unsigned char transmissionBeginPage = 3;
const unsigned char transmissionLocate = 4;
const unsigned char transmissionPartial = 5;
const unsigned char transmissionComplete = 6;


struct MemoryMapHeader
{
	size_t readLocation;
	size_t writeLocation;
	size_t viewerOpenFlag;
	size_t locateModeFlag;
	PLGraphicsIn graphicsIn;
};

const PLINT plMemoryMapReservedSpace = sizeof( MemoryMapHeader );

class PLMemoryMap
{
public:
	PLMemoryMap();
	PLMemoryMap( const char *name, PLINT size, bool mustExist, bool mustNotExist );
	void create( const char *name, PLINT size, bool mustExist, bool mustNotExist );
	void close();
	~PLMemoryMap();
	char *getBuffer() { return (char*)m_buffer; }
	bool isValid() {return m_buffer != NULL; }
	size_t getSize() {return m_size; }
private:
#ifdef WIN32
	HANDLE m_mapFile;
#else
	int m_mapFile;
	char * m_name;
#endif
	size_t m_size;
	void *m_buffer;
};

class PLNamedMutex
{
public:
	PLNamedMutex();
	PLNamedMutex( const char *name, bool aquireOnCreate = false );
	~PLNamedMutex();
	void create(const char *name, bool aquireOnCreate = false );
	void clear();
	void aquire();
	bool aquire( unsigned long millisecs );
	bool aquireNoWait();
	void release();
	bool isValid();
private:
	bool m_haveLock;
#ifdef WIN32
	HANDLE m_mutex;
#else if POSIX_HAS_SEMAPHORES
	sem_t * m_mutex;
#endif
};

class PLNamedMutexLocker
{
public:
	PLNamedMutexLocker( PLNamedMutex *mutex );
	~PLNamedMutexLocker();
private:
	 PLNamedMutex *m_mutex;
	 //remove default constructors
	 PLNamedMutexLocker();
	 PLNamedMutexLocker( const PLNamedMutexLocker & );
	 PLNamedMutexLocker & operator = ( const PLNamedMutex & );
};

#endif