// Copyright (C) 2015-2017 Phil Rosenberg
// Copyright (C) 2017 Alan W. Irwin
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

#ifdef PL_WXWIDGETS_IPC2

// Default constructor: Initialize m_wsem, m_rsem, and m_tsem to
// NULL to mark those as invalid semaphore locations.
PLThreeSemaphores::PLThreeSemaphores()
{
    m_wsem = NULL;
    m_rsem = NULL;
    m_tsem = NULL;
}

// Update the m_wsem, m_rsem, and m_tsem locations and
// conditionally (when mustExist is false) initialize those
// locations as semaphores.
void PLThreeSemaphores::initializeToValid( sem_t *wsem, sem_t *rsem, sem_t *tsem, bool mustExist )
{
    if ( wsem == NULL || rsem == NULL || tsem == NULL )
        throw ( "PLThreeSemaphores::initializeToValid: the requested sem_t * arguments must be non-NULL." );

    if ( !mustExist )
    {
        // Check the current semaphores are not valid.
        if ( areSemaphoresValid() )
            throw ( "PLThreeSemaphores::initializeToValid, mustExist false: attempt to initialize already valid semaphores" );
        m_wsem = wsem;
        m_rsem = rsem;
        m_tsem = tsem;
        // Middle argument of 1 ==> semaphore resident in shared memory.
        // Last argument ==> Both read and write semaphores initially blocked, transmit semaphore
        // initially unblocked.
        if ( sem_init( m_wsem, 1, 0 ) || sem_init( m_rsem, 1, 0 ) || sem_init( m_tsem, 1, 1 ) )
            throw ( "PLThreeSemaphores::initializeToValid, mustExist false: sem_init failed for at least one of the three semaphores" );
    }
    else
    {
        m_wsem = wsem;
        m_rsem = rsem;
        m_tsem = tsem;
        int value;
        // We want to test that these are semaphore locations that
        // have already been properly initialized in blocked state as above.
        // Attempt to test that assumption with sem_getvalue, but I
        // have gdb evidence that at least one OS implementation (that on Linux)
        // of sem_getvalue does not check that the given location is
        // a valid semaphore, and it is fairly likely in that case that
        // you will return a value of 0 so this test is not as rigourous as
        // it should be.
        if ( !( sem_getvalue( m_wsem, &value ) == 0 && value == 0 && sem_getvalue( m_rsem, &value ) == 0 && value == 0 && sem_getvalue( m_tsem, &value ) == 0 && value == 1 ) )
            throw( "PLThreeSemaphores::initializeToValid, mustExist true: at least one semaphore not valid or not set at correct initial value" );
    }
}

// Only destructor
PLThreeSemaphores::~PLThreeSemaphores()
{
    initializeToInvalid();
}

// If the m_wsem, m_rsem, and m_tsem locations are non-NULL
// destroy those semaphores.  Also, unconditionally set
// m_wsem, m_rsem, and m_tsem to NULL to mark those as invalid
// semaphore locations.
void PLThreeSemaphores::initializeToInvalid()
{
    if ( areSemaphoresValid() )
    {
        if ( sem_destroy( m_wsem ) )
            throw( "PLThreeSemaphores::initializeToInvalid: sem_destroy failed on write semaphore" );
        if ( sem_destroy( m_rsem ) )
            throw( "PLThreeSemaphores::initializeToInvalid: sem_destroy failed on read semaphore" );
        if ( sem_destroy( m_tsem ) )
            throw( "PLThreeSemaphores::initializeToInvalid: sem_destroy failed on transmit semaphore" );
    }
    m_wsem = NULL;
    m_rsem = NULL;
    m_tsem = NULL;
}

// Attempts to test semaphore validity using sem_getvalue on Linux
// proved fruitless since as far as I can tell with gdb that function
// always returns zero, i.e., always signals success ___so long as its
// sem_t * argument points to _any_ non-NULL accessible memory area that is
// cast to sem_t *__!  And when called with a NULL argument sem_getvalue
// segfaults rather than returning with a non-zero value!  So Linux
// sem_getvalue is pretty crude.

// So instead of checking the return value of sem_getvalue, we instead
// only check whether m_wsem and m_rsem are not NULL (signalling
// valid) or NULL (signalling invalid).

// N.B. the default PLThreeSemaphores constructor sets these locations
// to NULL, and the alternative constructor or initializeToValid must
// be called with mustExist argument false to change these locations
// to valid semaphores that are initially blocked.

bool PLThreeSemaphores::isWriteSemaphoreValid()
{
    return m_wsem != NULL;
}

bool PLThreeSemaphores::isReadSemaphoreValid()
{
    return m_rsem != NULL;
}

bool PLThreeSemaphores::isTransmitSemaphoreValid()
{
    return m_tsem != NULL;
}

// Return true if all semaphores are valid.
// Return false if all semaphores are invalid.
// Throw an exception otherwise.

bool PLThreeSemaphores::areSemaphoresValid()
{
    if ( isWriteSemaphoreValid() && isReadSemaphoreValid() && isTransmitSemaphoreValid() )
    {
        return true;
    }
    else if ( !isWriteSemaphoreValid() && !isReadSemaphoreValid() && !isTransmitSemaphoreValid() )
    {
        return false;
    }

    throw( "PLThreeSemaphores::areSemaphoresValid: invalid combination of read, write, and transmit semaphore validity" );

    // Should never reach this statement, but include it anyway to quiet possible compiler warnings.
    return false;
}

// Check whether write and read semaphores are valid and blocked.
bool PLThreeSemaphores::areWriteReadSemaphoresBlocked()
{
    if ( areSemaphoresValid() )
    {
        int wvalue, rvalue;
        // We want to test that these are semaphore locations that
        // have already been properly initialized in blocked state as above.
        // Attempt to test that assumption with sem_getvalue, but I
        // have gdb evidence that at least one OS implementation (that on Linux)
        // of sem_getvalue does not check that the given location is
        // a valid semaphore, and it is fairly likely in that case that
        // you will return a value of 0 so this test is not as rigourous as
        // it should be.
        if ( sem_getvalue( m_wsem, &wvalue ) != 0 || sem_getvalue( m_rsem, &rvalue ) != 0 )
            throw( "PLThreeSemaphores::areSemaphoresBlocked: sem_getvalue error on one of the write or read semaphores" );
        if ( wvalue == 0 && rvalue == 0 )
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

// Get value of Write semaphore.
int PLThreeSemaphores::getValueWriteSemaphore()
{
    // Initialize to wacko value to quiet compiler uninitialized complaints
    // for the case of the throws below.
    int ret_value = -42;
    if ( !isWriteSemaphoreValid() )
    {
        throw( "PLThreeSemaphores::getValueWriteSemaphore: attempt to get value for invalid semaphore." );
    }
    else
    {
        if ( sem_getvalue( m_wsem, &ret_value ) != 0 )
            throw( "PLThreeSemaphores::getValueWriteSemaphore: sem_getvalue failed" );
    }
    return ret_value;
}

// Get value of Read semaphore.
int PLThreeSemaphores::getValueReadSemaphore()
{
    // Initialize to wacko value to quiet compiler uninitialized complaints
    // for the case of the throws below.
    int ret_value = -42;
    if ( !isReadSemaphoreValid() )
    {
        throw( "PLThreeSemaphores::getValueReadSemaphore: attempt to get value for invalid semaphore." );
    }
    else
    {
        if ( sem_getvalue( m_rsem, &ret_value ) != 0 )
            throw( "PLThreeSemaphores::getValueReadSemaphore: sem_getvalue failed" );
    }
    return ret_value;
}

void PLThreeSemaphores::postWriteSemaphore()
{
    if ( !isWriteSemaphoreValid() )
        throw( "PLThreeSemaphores::postWriteSemaphore: invalid write semaphore" );

    if ( sem_post( m_wsem ) )
        throw( "PLThreeSemaphores::postWriteSemaphore: sem_post failed for write semaphore" );
}

void PLThreeSemaphores::waitWriteSemaphore()
{
    if ( !isWriteSemaphoreValid() )
        throw( "PLThreeSemaphores::waitWriteSemaphore: invalid write semaphore" );

    if ( sem_wait( m_wsem ) )
        throw( "PLThreeSemaphores::waitWriteSemaphore: sem_wait failed for write semaphore" );
}

void PLThreeSemaphores::postReadSemaphore()
{
    if ( !isReadSemaphoreValid() )
        throw( "PLThreeSemaphores::postReadSemaphore: invalid read semaphore" );

    if ( sem_post( m_rsem ) )
        throw( "PLThreeSemaphores::postReadSemaphore: sem_post failed for read semaphore" );
}

void PLThreeSemaphores::waitReadSemaphore()
{
    if ( !isReadSemaphoreValid() )
        throw( "PLThreeSemaphores::waitReadSemaphore: invalid read semaphore" );

    if ( sem_wait( m_rsem ) )
        throw( "PLThreeSemaphores::waitReadSemaphore: sem_wait failed for read semaphore" );
}

void PLThreeSemaphores::postTransmitSemaphore()
{
    if ( !isTransmitSemaphoreValid() )
        throw( "PLThreeSemaphores::postTransmitSemaphore: invalid transmit semaphore" );

    if ( sem_post( m_tsem ) )
        throw( "PLThreeSemaphores::postTransmitSemaphore: sem_post failed for transmit semaphore" );
}

void PLThreeSemaphores::waitTransmitSemaphore()
{
    if ( !isTransmitSemaphoreValid() )
        throw( "PLThreeSemaphores::waitTransmitSemaphore: invalid transmit semaphore" );

    if ( sem_wait( m_tsem ) )
        throw( "PLThreeSemaphores::waitTransmitSemaphore: sem_wait failed for transmit semaphore" );
}
#endif //#ifdef PL_WXWIDGETS_IPC2

//--------------------------------------------------------------------------
//  Constructor, creates the object but does not actually create or link to
//  any shared memory.
//--------------------------------------------------------------------------
PLMemoryMap::PLMemoryMap()
{
#ifdef WIN32
    m_mapFile = NULL;
#else
    m_mapFile = -1;
    m_name    = NULL;
#endif
    m_buffer = NULL;
    m_size   = 0;
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
    m_name    = NULL;
#endif
    m_buffer = NULL;
    m_size   = 0;
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
    assert( !( mustExist && mustNotExist ) );
    if ( mustExist && mustNotExist )
        return;
#ifdef WIN32
    if ( mustExist )
        m_mapFile = OpenFileMappingA( FILE_MAP_ALL_ACCESS, FALSE, name );
    else if ( mustNotExist )
    {
        m_mapFile = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL,
            PAGE_READWRITE, 0, size, name );
        if ( GetLastError() == ERROR_ALREADY_EXISTS )
            close();
    }
    else
        m_mapFile = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL,
            PAGE_READWRITE, 0, size, name );

    if ( m_mapFile )
        m_buffer = MapViewOfFile( m_mapFile, FILE_MAP_ALL_ACCESS, 0, 0, size );
#else
    if ( mustExist )
    {
        m_mapFile = shm_open( name, O_RDWR, 0 );
    }
    else if ( mustNotExist )
    {
        m_mapFile = shm_open( name, O_RDWR | O_CREAT | O_EXCL, S_IRWXU );     //S_IRWXU gives user wrx permissions
        if ( ftruncate( m_mapFile, size ) == -1 )
            close( );
    }
    else
    {
        m_mapFile = shm_open( name, O_RDWR | O_CREAT, S_IRWXU );       //S_IRWXU gives user wrx permissions
        if ( ftruncate( m_mapFile, size ) == -1 )
            close( );
    }
    if ( m_mapFile != -1 )
    {
        m_buffer = mmap( NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_mapFile, 0 );
        m_name   = new char[strlen( name ) + 1];
        strcpy( m_name, name );
    }
#endif
    if ( isValid() )
        m_size = size;
}

#ifdef PL_WXWIDGETS_IPC2

// This IPC method is an adaptation of the method used in
// cmake/test_linux_ipc/pshm_write.c.

// This transmitBytes method on the transmitting side should be used
// in tandem with the receiveBytes method on the receiving side.

// Transmit data via three-semaphore IPC from the transmitting side to
// the receiving side. It is the responsibility of transmitBytes to
// check the semaphores are in the correct blocked state at the start
// and end of the transmission of data.

// In the three-semaphores method of IPC, the shared memory area must
// correspond to the shmbuf struct which contains some control data
// explicitly used for the communication, e.g., at least the total
// number of bytes of data to be transferred, and limited size
// non-control data areas to be used for transferring an unlimited
// number of data bytes.

// If ifHeader is true, then src is a MemoryMapHeader header
// which is transferred to the corresponding area of shared memory
// (the internal dest in this case).  Otherwise, src is a char array
// of unlimited size whose transfer is staged through the data area
// (the internal dest in this case) of shared memory.

// The src argument must always be a pointer to general rather than
// shared memory to avoid overlaps between src and internal dest in
// shared memory not only for code logic clarity but also because
// memcpy rather than memmove is used inside transmitBytes.

// n is the total number of bytes that will be moved.

void PLMemoryMap::transmitBytes( bool ifHeader, const void *src, size_t n )
{
    size_t     chunk, nbytes_chunk, transmitted_bytes;
    const char * csrc  = (const char *) src;
    void       * hdest = (void *) getHeader();
    void       * bdest = (void *) getBuffer();

    if ( !isValid() )
        throw ( "PLMemoryMap::transmitBytes: invalid memory map" );

    size_t size_area;
    if ( ifHeader )
        size_area = sizeof ( MemoryMapHeader );
    else
        size_area = PL_SHARED_ARRAY_SIZE;

    if ( ifHeader && n != sizeof ( MemoryMapHeader ) )
        throw( "PLMemoryMap::transmitBytes: ifHeader true has invalid n value" );

    // Wait until previous call (by either side) of transmitBytes has been completed
    // to avoid a potential race condition.
    m_threeSemaphores.waitTransmitSemaphore();

    if ( !m_threeSemaphores.areWriteReadSemaphoresBlocked() )
        throw( "PLMemoryMap::transmitBytes: attempt to start transfer with semaphores not in correct blocked state." );
    // Receiving side is blocked and initialize this transmitting side to go first.
    m_threeSemaphores.postWriteSemaphore();

    for ( chunk = 0, transmitted_bytes = 0;; chunk++, csrc += nbytes_chunk )
    {
        // Wait for our turn to change the shared memory shmbuf.
        m_threeSemaphores.waitWriteSemaphore();

        if ( chunk == 0 )
        {
            // Update nbytes control data part of that shared memory shmbuf.
            ( (shmbuf *) m_buffer )->nbytes = n;
        }

        nbytes_chunk = MIN( size_area, n - transmitted_bytes );
        if ( nbytes_chunk > 0 )
        {
            if ( ifHeader )
                memcpy( hdest, csrc, nbytes_chunk );
            else
                memcpy( bdest, csrc, nbytes_chunk );
        }

        // Give the receiveBytes method a turn to process the shared
        // memory shmbuf we have just changed.
        m_threeSemaphores.postReadSemaphore();

        if ( !( nbytes_chunk > 0 ) )
        {
            break;
        }
        transmitted_bytes += nbytes_chunk;
    }

    // All shared memory shmbuf changes have been transmitted so wait
    // for receiveBytes to process the last of those.
    m_threeSemaphores.waitWriteSemaphore();

    // If the transfer has been a success, then write and read semaphores should
    // end up as blocked.  Check that:
    if ( !m_threeSemaphores.areWriteReadSemaphoresBlocked() )
        throw( "PLMemoryMap::transmitBytes (internal error): transfer finished with write and read semaphores not in correct blocked state." );

    // Allow a subsequent call of transmitBytes (by either side) to start executing.
    m_threeSemaphores.postTransmitSemaphore();
}

// This IPC method is an adaptation of the method used in
// cmake/test_linux_ipc/pshm_read.c.

// This receiveBytes method on the receiving side should be used in
// tandem with the transmitBytes method on the transmitting side.

// Receive data via three-semaphore IPC from the transmitting side.

// In the three-semaphores method of IPC, the shared memory area must
// correspond to the shmbuf struct which contains some control data
// explicitly used for the communication, e.g., at least the total
// number of bytes of data to be transferred, and limited size
// non-control data areas to be used for transferring an unlimited
// number of data bytes.

// if ifHeader is true, then (the internal) src is the MemoryMapHeader
// header area of shared memory which is transferred to a
// corresponding area
// pointed to by the dest argument.  Otherwise, (the internal) src is
// the char * data area of shared memory which is a staging area for
// the transfer of n bytes into the location pointed to by dest.

// The dest argument must always be a pointer to general rather than
// shared memory to avoid overlaps between internal src in shared
// memory and dest not only for code logic clarity but also because
// memcpy rather than memmove is used inside receiveBytes.

// n is the total number of bytes that will be moved.

void PLMemoryMap::receiveBytes( bool ifHeader, void *dest, size_t n )
{
    size_t chunk, nbytes, nbytes_chunk, received_bytes;
    char   * cdest = (char *) dest;
    void   * hsrc  = (void *) getHeader();
    void   * bsrc  = (void *) getBuffer();

    if ( !isValid() )
        throw( "PLMemoryMap::receiveBytes: invalid memory map" );

    size_t size_area;
    if ( ifHeader )
        size_area = sizeof ( MemoryMapHeader );
    else
        size_area = PL_SHARED_ARRAY_SIZE;

    if ( ifHeader && n != sizeof ( MemoryMapHeader ) )
        throw( "PLMemoryMap::receiveBytes: ifHeader true has invalid n value" );
    // N.B. it is the responsibility of transmitBytes to initialize the semaphores
    // to the correct values, but we at least check here that the semaphores are valid.
    m_threeSemaphores.areSemaphoresValid();

    for ( chunk = 0, received_bytes = 0;; chunk++, cdest += nbytes_chunk )
    {
        // Wait for our turn to process the shared memory shmbuf that has been updated
        // by transmitBytes.
        m_threeSemaphores.waitReadSemaphore();

        if ( chunk == 0 )
        {
            // Update *nbytes from the nbytes control data part of that shared memory shmbuf.
            nbytes = ( (shmbuf *) m_buffer )->nbytes;
            if ( nbytes > n )
                throw ( "PLMemoryMap::receiveBytes: n too small to receive results" );
        }

        nbytes_chunk = MIN( size_area, nbytes - received_bytes );
        if ( !( nbytes_chunk > 0 ) )
        {
            break;
        }
        else
        {
            received_bytes += nbytes_chunk;

            if ( ifHeader )
                memcpy( cdest, hsrc, nbytes_chunk );
            else
                memcpy( cdest, bsrc, nbytes_chunk );
            // Give the transmitter a turn to send another chunk of bytes.
            m_threeSemaphores.postWriteSemaphore();
        }
    }
    // All chunks have been received and processed so signal transmitter
    // we are done.
    m_threeSemaphores.postWriteSemaphore();

    // The transmitBytes checks after all transactions with this
    // receiveBytes routine are completed that the semaphores are in
    // the correct blocked state.  So there is nothing further for us
    // to check here.
}
#endif // #ifdef PL_WXWIDGETS_IPC2
//--------------------------------------------------------------------------
//  Close an area of mapped memory. When all processes have closed their
//  connections the area will be removed by the OS.
//--------------------------------------------------------------------------
void PLMemoryMap::close()
{
#ifdef WIN32
    if ( m_buffer )
        UnmapViewOfFile( m_buffer );
    if ( m_mapFile )
        CloseHandle( m_mapFile );
    m_mapFile = NULL;
#else
    if ( m_buffer )
    {
        munmap( m_buffer, m_size );
    }
    if ( m_mapFile != -1 )
    {
        shm_unlink( m_name );
    }
    if ( m_name )
    {
        delete[] m_name;
    }
    m_mapFile = -1;
    m_name    = NULL;

#endif
    m_buffer = NULL;
    m_size   = 0;
}

//--------------------------------------------------------------------------
//  Destructor, closes the connection to the mapped memory.
//--------------------------------------------------------------------------
PLMemoryMap::~PLMemoryMap()
{
    close();
}

#ifndef PL_WXWIDGETS_IPC2

PLNamedMutex::PLNamedMutex()
{
    m_mutex    = NULL;
    m_haveLock = false;
}

PLNamedMutex::PLNamedMutex( const char *name, bool aquireOnCreate )
{
    m_mutex    = NULL;
    m_haveLock = false;
    create( name, aquireOnCreate );
}

void PLNamedMutex::create( const char *name, bool aquireOnCreate )
{
#ifdef WIN32
    m_mutex = CreateMutexA( NULL, aquireOnCreate ? TRUE : FALSE, name );
#else
    m_mutex        = NULL;
    m_mutexName[0] = '/';
    strncpy( m_mutexName + 1, name, 250 );
    m_mutexName[250] = '\0';
    m_mutex          = sem_open( m_mutexName, O_CREAT, S_IRWXU, 1 );
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
    if ( !m_haveLock )
        throw( result );
}

bool PLNamedMutex::aquire( unsigned long millisecs )
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
    if ( !m_haveLock )
        return;
#ifdef WIN32
    if ( m_mutex )
        ReleaseMutex( m_mutex );
#else
    sem_post( m_mutex );
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
    // Needed to release shared memory resources used by named semaphores.
    sem_unlink( m_mutexName );
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
    m_mutex->release();
    m_mutex = NULL;
}
#endif //#ifndef PL_WXWIDGETS_IPC2
