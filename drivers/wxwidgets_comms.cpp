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
#include <wx/utils.h> //Needed for wxMilliSleep
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

// This moveBytesWriter method on the writer side
// should be used in tandem with the moveBytesReader
// method on the reader side.

// Transmit data via two-semaphore IPC from the writer side (i.e., the
// side that creates the shared memory area and the shared semaphores
// (unnamed or named)) to the reader side (i.e., the side that checks
// the shared memory area and shared semaphores that have been created
// by the writer side).  It is the responsibility of moveBytesWriter to
// initialize the semaphores to the correct values, and check at the
// end that they are left in the correct blocked state.

// In the two-semaphores method of IPC, the shared memory area must
// correspond to the shmbuf struct which contains some control data
// explicitly used for the communication, e.g., at least the total
// number of bytes of data to be transferred, and limited size
// non-control data areas to be used for transferring an unlimited
// number of data bytes.

// if ifHeader is true, then src is a MemoryMapHeader header which is
// transferred to the corresponding area of shared memory (the
// internal dest in this case).  Otherwise, src is a char array of
// unlimited size whose transfer is staged through the data area (the
// internal dest in this case) of shared memory.

// n is the total number of bytes that will be moved.

// N.B. memcpy used inside so src cannotoverlap with (the internal) dest.

void PLMemoryMap::moveBytesWriter( bool ifHeader, const void *src, size_t n )
{
    size_t     chunk, nbytes_chunk, transmitted_bytes;
    const char * csrc  = (const char *) src;
    void       * hdest = (void *) getHeader();
    void       * bdest = (void *) getBuffer();

    if ( !isValid() )
        throw ( "PLMemoryMap::moveBytesWriter: invalid memory map" );

    size_t size_area;
    if ( ifHeader )
        size_area = sizeof ( MemoryMapHeader );
    else
        size_area = PL_SHARED_ARRAY_SIZE;

    if ( ifHeader && n != sizeof ( MemoryMapHeader ) )
        throw( "PLMemoryMap::moveBytesWriter: ifHeader true has invalid n value" );

    if ( getReverseDataFlow() )
    {
        // The previous data transfer was initiated with moveBytesReaderReversed
        // so must wait for the OS scheduler to let that routine finish to
        // avoid a race condition.
        // Mark this forthcoming data transfer as in the non-reversed direction.
        setReverseDataFlow( false );
        // Attempt to avoid the above race by using the wxwidgets
        // library to wait (at least) 10 system ticks on millsecond tick
        // systems and much more than that on systems with higher tick
        // resolutions.  So it is likely this is way more than enough.
        // Note, this code is only executed when the direction of data
        // flow is disrupted, and usually that only occurs for an
        // initialization handshake and interactive use of -dev
        // wxwidgets.  For both cases we should not be concerned about
        // speed.
        wxMilliSleep( 10 );
    }
    if ( !m_twoSemaphores.areBothSemaphoresBlocked() )
        throw( "PLMemoryMap::moveBytesWriter: attempt to start transfer with semaphores not in correct blocked state (likely due to a race condition)." );
    // Read side is blocked and initialize write side to go first.
    m_twoSemaphores.postWriteSemaphore();

    for ( chunk = 0, transmitted_bytes = 0;; chunk++, csrc += nbytes_chunk )
    {
        // Wait for our turn to change the shared memory shmbuf.
        m_twoSemaphores.waitWriteSemaphore();

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

        // Give the moveBytesReader method a turn to process the shared
        // memory shmbuf we have just changed.
        m_twoSemaphores.postReadSemaphore();

        if ( !( nbytes_chunk > 0 ) )
        {
            break;
        }
        transmitted_bytes += nbytes_chunk;
    }

    // All shared memory shmbuf changes have been transmitted so wait
    // for moveBytesReader to process the last of those.
    m_twoSemaphores.waitWriteSemaphore();

    // If the transfer has been a success, then both semaphores should
    // end up as blocked.  Check that:
    if ( !m_twoSemaphores.areBothSemaphoresBlocked() )
        throw( "PLMemoryMap::moveBytesWriter (internal error): transfer finished with semaphores not in correct blocked state." );
}

// This IPC method is an adaptation of the method used in
// cmake/test_linux_ipc/pshm_read.c.

// This moveBytesReader method on the reader side
// should be used in tandem with the moveBytesWriter
// method on the writer side.

// Receive data via two-semaphore IPC from the writer side (i.e., the
// side that creates the shared memory area and the shared semaphores
// (unnamed or named)) that is sent to the reader side (i.e., the side
// that checks the shared memory area and shared semaphores that have
// been created by the writer side).

// In the two-semaphores method of IPC, the shared memory area must
// correspond to the shmbuf struct which contains some control data
// explicitly used for the communication, e.g., at least the total
// number of bytes of data to be transferred, and limited size
// non-control data areas to be used for transferring an unlimited
// number of data bytes.

// if ifHeader is true, then (the internal) src is the MemoryMapHeader
// header area of shared memory which is transferred to a
// corresponding area (in general memory or the shared memory header)
// pointed to by the dest argument.  Otherwise, (the internal) src is
// the char * data area of shared memory which is a staging area for
// the transfer of n bytes into the location pointed to by dest.

// dest must be a pointer to a destination address which can be in
// general memory or correspond to a data area within the shared
// memory area.

// n is the total number of bytes that will be moved.

// N.B. memcpy used inside so (internal) src cannot overlap with dest.

void PLMemoryMap::moveBytesReader( bool ifHeader, void *dest, size_t n )
{
    size_t chunk, nbytes, nbytes_chunk, received_bytes;
    char   * cdest = (char *) dest;
    void   * hsrc  = (void *) getHeader();
    void   * bsrc  = (void *) getBuffer();

    if ( !isValid() )
        throw( "PLMemoryMap::moveBytesReader: invalid memory map" );

    size_t size_area;
    if ( ifHeader )
        size_area = sizeof ( MemoryMapHeader );
    else
        size_area = PL_SHARED_ARRAY_SIZE;

    if ( ifHeader && n != sizeof ( MemoryMapHeader ) )
        throw( "PLMemoryMap::moveBytesReader: ifHeader true has invalid n value" );
    // N.B. it is the responsibility of moveBytesWriter to initialize the semaphores
    // to the correct values, but we at least check here that the semaphores are valid.
    m_twoSemaphores.areBothSemaphoresValid();

    for ( chunk = 0, received_bytes = 0;; chunk++, cdest += nbytes_chunk )
    {
        // Wait for our turn to process the shared memory shmbuf that has been updated
        // by moveBytesWriter.
        m_twoSemaphores.waitReadSemaphore();

        if ( chunk == 0 )
        {
            // Update *nbytes from the nbytes control data part of that shared memory shmbuf.
            nbytes = ( (shmbuf *) m_buffer )->nbytes;
            if ( nbytes > n )
                throw ( "PLMemoryMap::moveBytesReader: n too small to receive results" );
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
            m_twoSemaphores.postWriteSemaphore();
        }
    }
    // All chunks have been received and processed so signal transmitter
    // we are done.
    m_twoSemaphores.postWriteSemaphore();

    // The moveBytesWriter checks after all transactions with this
    // moveBytesReader routine are completed that the semaphores are
    // in the correct blocked state.  So there is nothing for us to
    // check here.
}

// This IPC method is an adaptation of the method used in
// cmake/test_linux_ipc/pshm_write.c.
// N.B. The moveBytesReaderReversed method is identical to
// moveBytesWriter except that the roles of the write and
// read semaphores have been swapped so this can be used
// to send data from the reader side to the writer side.

// This moveBytesReaderReversed method on the reader side
// should be used in tandem with the moveBytesWriterReversed
// method on the writer side.

// Transmit data via two-semaphore IPC to the writer side (i.e., the
// side that creates the shared memory area and the shared semaphores
// (unnamed or named)) from the reader side (i.e., the side that checks
// the shared memory area and shared semaphores that have been created
// by the writer side).  It is the responsibility of moveBytesReaderReversed to
// initialize the semaphores to the correct values, and check at the
// end that they are left in the correct blocked state.

// In the two-semaphores method of IPC, the shared memory area must
// correspond to the shmbuf struct which contains some control data
// explicitly used for the communication, e.g., at least the total
// number of bytes of data to be transferred, and limited size
// non-control data areas to be used for transferring an unlimited
// number of data bytes.

// if ifHeader is true, then src is a MemoryMapHeader header which is
// transferred to the corresponding area of shared memory (the
// internal dest in this case).  Otherwise, src is a char array of
// unlimited size whose transfer is staged through the data area (the
// internal dest in this case) of shared memory.

// n is the total number of bytes that will be moved.

// N.B. memcpy used inside so src cannot overlap with (the internal) dest.

void PLMemoryMap::moveBytesReaderReversed( bool ifHeader, const void *src, size_t n )
{
    size_t     chunk, nbytes_chunk, transmitted_bytes;
    const char * csrc  = (const char *) src;
    void       * hdest = (void *) getHeader();
    void       * bdest = (void *) getBuffer();

    if ( !isValid() )
        throw ( "PLMemoryMap::moveBytesReaderReversed: invalid memory map" );

    size_t size_area;
    if ( ifHeader )
        size_area = sizeof ( MemoryMapHeader );
    else
        size_area = PL_SHARED_ARRAY_SIZE;

    if ( ifHeader && n != sizeof ( MemoryMapHeader ) )
        throw( "PLMemoryMap::moveBytesReaderReversed: ifHeader true has invalid n value" );

    if ( !getReverseDataFlow() )
    {
        // The previous data transfer was initiated with moveBytesWriter
        // so must wait for the OS scheduler to let that routine finish to
        // avoid a race condition.
        // Mark this forthcoming data transfer as in the reversed direction.
        setReverseDataFlow( true );
        // Attempt to avoid the above race by using the wxwidgets
        // library to wait (at least) 10 system ticks on millsecond tick
        // systems and much more than that on systems with higher tick
        // resolutions.  So it is likely this is way more than enough.
        // Note, this code is only executed when the direction of data
        // flow is disrupted, and usually that only occurs for an
        // initialization handshake and interactive use of -dev
        // wxwidgets.  For both cases we should not be concerned about
        // speed.
        wxMilliSleep( 10 );
    }
    if ( !m_twoSemaphores.areBothSemaphoresBlocked() )
        throw( "PLMemoryMap::moveBytesReaderReversed: attempt to start transfer with semaphores not in correct blocked state (likely due to a race condition." );
    // Write side is blocked and initialize read side to go first.
    m_twoSemaphores.postReadSemaphore();

    for ( chunk = 0, transmitted_bytes = 0;; chunk++, csrc += nbytes_chunk )
    {
        // Wait for our turn to change the shared memory shmbuf.
        m_twoSemaphores.waitReadSemaphore();

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

        // Give the moveBytesWriter method a turn to process the shared
        // memory shmbuf we have just changed.
        m_twoSemaphores.postWriteSemaphore();

        if ( !( nbytes_chunk > 0 ) )
        {
            break;
        }
        transmitted_bytes += nbytes_chunk;
    }

    // All shared memory shmbuf changes have been transmitted so wait
    // for moveBytesWriterReversed to process the last of those.
    m_twoSemaphores.waitReadSemaphore();

    // If the transfer has been a success, then both semaphores should
    // end up as blocked.  Check that:
    if ( !m_twoSemaphores.areBothSemaphoresBlocked() )
        throw( "PLMemoryMap::moveBytesReaderReversed (internal error): transfer finished with semaphores not in correct blocked state." );
}

// This IPC method is an adaptation of the method used in
// cmake/test_linux_ipc/pshm_read.c.
// N.B. The moveBytesWriterReversed method is identical to
// moveBytesReader except that the roles of the write and
// read semaphores have been swapped so this can be used
// to receive data on the writer side that is sent
// from the reader side.

// This moveBytesWriterReversed method on the writer side
// should be used in tandem with the moveBytesReaderReversed
// method on the reader side.

// Receive data via two-semaphore IPC on the writer side (i.e., the
// side that creates the shared memory area and the shared semaphores
// (unnamed or named)) that is sent from the reader side (i.e., the side
// that checks the shared memory area and shared semaphores that have
// been created by the writer side).

// In the two-semaphores method of IPC, the shared memory area must
// correspond to the shmbuf struct which contains some control data
// explicitly used for the communication, e.g., at least the total
// number of bytes of data to be transferred, and limited size
// non-control data areas to be used for transferring an unlimited
// number of data bytes.

// if ifHeader is true, then (the internal) src is the MemoryMapHeader
// header area of shared memory which is transferred to a
// corresponding area (in general memory or the shared memory header)
// pointed to by the dest argument.  Otherwise, (the internal) src is
// the char * data area of shared memory which is a staging area for
// the transfer of n bytes into the location pointed to by dest.

// dest must be a pointer to a destination address which can be in
// general memory or correspond to a data area within the shared
// memory area.

// n is the total number of bytes that will be moved.

// N.B. memcpy used inside so (internal) src cannot overlap with dest.

void PLMemoryMap::moveBytesWriterReversed( bool ifHeader, void *dest, size_t n )
{
    size_t chunk, nbytes, nbytes_chunk, received_bytes;
    char   * cdest = (char *) dest;
    void   * hsrc  = (void *) getHeader();
    void   * bsrc  = (void *) getBuffer();

    if ( !isValid() )
        throw( "PLMemoryMap::moveBytesWriterReversed: invalid memory map" );

    size_t size_area;
    if ( ifHeader )
        size_area = sizeof ( MemoryMapHeader );
    else
        size_area = PL_SHARED_ARRAY_SIZE;

    if ( ifHeader && n != sizeof ( MemoryMapHeader ) )
        throw( "PLMemoryMap::moveBytesWriterReversed: ifHeader true has invalid n value" );
    // N.B. it is the responsibility of PLMemoryMap::moveBytesReaderReversed to initialize the semaphores
    // to the correct values, but we at least check here that the semaphores are valid.
    m_twoSemaphores.areBothSemaphoresValid();

    for ( chunk = 0, received_bytes = 0;; chunk++, cdest += nbytes_chunk )
    {
        // Wait for our turn to process the shared memory shmbuf that has been updated
        // by moveBytesReaderReversed.
        m_twoSemaphores.waitWriteSemaphore();

        if ( chunk == 0 )
        {
            // Update *nbytes from the nbytes control data part of that shared memory shmbuf.
            nbytes = ( (shmbuf *) m_buffer )->nbytes;
            if ( nbytes > n )
                throw ( "PLMemoryMap::moveBytesWriterReversed: n too small to receive results" );
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
            m_twoSemaphores.postReadSemaphore();
        }
    }
    // All chunks have been received and processed so signal transmitter
    // we are done.
    m_twoSemaphores.postReadSemaphore();

    // The moveBytesReaderReversed routine checks after all transactions with this
    // moveBytesWriterReversed routine are completed that the semaphores are
    // in the correct blocked state.  So there is nothing for us to
    // check here.
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

#ifdef PL_WXWIDGETS_IPC2

// Default constructor: Initialize m_wsem and m_rsem to NULL;
PLTwoSemaphores::PLTwoSemaphores()
{
    m_wsem = NULL;
    m_rsem = NULL;
}

// Update the m_wsem and m_rsem locations and conditionally (when
// mustExist is false) initialize those locations as semaphores.
void PLTwoSemaphores::initializeToValid( sem_t *wsem, sem_t *rsem, bool mustExist )
{
    if ( wsem == NULL || rsem == NULL )
        throw ( "PLTwoSemaphores::initializeToValid: both the requested wsem and rsem values must be non-NULL." );

    if ( !mustExist )
    {
        // Check the current semaphores are not valid.
        if ( areBothSemaphoresValid() )
            throw ( "PLTwoSemaphores::initializeToValid, mustExist false: attempt to initialize already valid semaphores" );
        m_wsem = wsem;
        m_rsem = rsem;
        // Middle argument of 1 ==> semaphore resident in shared memory.
        // Last argument of 0 ==> Both reader and writer initially blocked.
        if ( sem_init( m_wsem, 1, 0 ) || sem_init( m_rsem, 1, 0 ) )
            throw ( "PLTwoSemaphores::initializeToValid, mustExist false: sem_init failed for one of the two semaphores" );
    }
    else
    {
        m_wsem = wsem;
        m_rsem = rsem;
        int value;
        // We want to test that these are semaphore locations that
        // have already been properly initialized in blocked state as above.
        // Attempt to test that assumption with sem_getvalue, but I
        // have gdb evidence that at least one OS implementation (that on Linux)
        // of sem_getvalue does not check that the given location is
        // a valid semaphore, and it is fairly likely in that case that
        // you will return a value of 0 so this test is not as rigourous as
        // it should be.
        if ( !( sem_getvalue( m_wsem, &value ) == 0 && value == 0 && sem_getvalue( m_rsem, &value ) == 0 && value == 0 ) )
            throw( "PLTwoSemaphores::initializeToValid, mustExist true: semaphores not valid or not set at correct blocked values" );
    }
}

// Only destructor
PLTwoSemaphores::~PLTwoSemaphores()
{
    initializeToInvalid();
}

// Conditionally destroy the m_wsem and m_rsem locations as
// semaphores.  Unconditionally set both m_wsem and m_rsem to NULL
// to mark those as invalid semaphore locations.
void PLTwoSemaphores::initializeToInvalid()
{
    if ( areBothSemaphoresValid() )
    {
        if ( sem_destroy( m_wsem ) )
            throw( "PLTwoSemaphores::initializeToInvalid: sem_destroy failed on write semaphore" );
        if ( sem_destroy( m_rsem ) )
            throw( "PLTwoSemaphores::initializeToInvalid: sem_destroy failed on read semaphore" );
    }
    m_wsem = NULL;
    m_rsem = NULL;
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

// N.B. the default PLTwoSemaphores constructor sets these locations
// to NULL, and the alternative constructor or initializeToValid must
// be called with mustExist argument false to change these locations
// to valid semaphores that are initially blocked.

bool PLTwoSemaphores::isWriteSemaphoreValid()
{
    return m_wsem != NULL;
}

bool PLTwoSemaphores::isReadSemaphoreValid()
{
    return m_rsem != NULL;
}

// Return true if both semaphores are valid.
// Return false if both semaphores are invalid.
// Throw an exception otherwise.

bool PLTwoSemaphores::areBothSemaphoresValid()
{
    if ( isWriteSemaphoreValid() && isReadSemaphoreValid() )
    {
        return true;
    }
    else if ( !isWriteSemaphoreValid() && !isReadSemaphoreValid() )
    {
        return false;
    }

    throw( "PLTwoSemaphores::areBothSemaphoresValid: invalid combination of read and write semaphore validity" );

    // Should never reach this statement, but include it anyway to quiet possible compiler warnings.
    return false;
}

// Check whether both semaphores are valid and blocked.
bool PLTwoSemaphores::areBothSemaphoresBlocked()
{
    if ( areBothSemaphoresValid() )
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
            throw( "PLTwoSemaphores::areBothSemaphoresBlocked: sem_getvalue error on one of the semaphores" );
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
int PLTwoSemaphores::getValueWriteSemaphore()
{
    // Initialize to wacko value to quiet compiler uninitialized complaints
    // for the case of the throws below.
    int ret_value = -42;
    if ( !isWriteSemaphoreValid() )
    {
        throw( "PLTwoSemaphores::getValueWriteSemaphore: attempt to get value for invalid semaphore." );
    }
    else
    {
        if ( sem_getvalue( m_wsem, &ret_value ) != 0 )
            throw( "PLTwoSemaphores::getValueWriteSemaphore: sem_getvalue failed" );
    }
    return ret_value;
}

// Get value of Read semaphore.
int PLTwoSemaphores::getValueReadSemaphore()
{
    // Initialize to wacko value to quiet compiler uninitialized complaints
    // for the case of the throws below.
    int ret_value = -42;
    if ( !isReadSemaphoreValid() )
    {
        throw( "PLTwoSemaphores::getValueReadSemaphore: attempt to get value for invalid semaphore." );
    }
    else
    {
        if ( sem_getvalue( m_rsem, &ret_value ) != 0 )
            throw( "PLTwoSemaphores::getValueReadSemaphore: sem_getvalue failed" );
    }
    return ret_value;
}

void PLTwoSemaphores::postWriteSemaphore()
{
    if ( !isWriteSemaphoreValid() )
        throw( "PLTwoSemaphores::postWriteSemaphore: invalid write semaphore" );

    if ( sem_post( m_wsem ) )
        throw( "PLTwoSemaphores::postWriteSemaphore: sem_post failed for write semaphore" );
}

void PLTwoSemaphores::waitWriteSemaphore()
{
    if ( !isWriteSemaphoreValid() )
        throw( "PLTwoSemaphores::waitWriteSemaphore: invalid write semaphore" );

    if ( sem_wait( m_wsem ) )
        throw( "PLTwoSemaphores::waitWriteSemaphore: sem_wait failed for write semaphore" );
}

void PLTwoSemaphores::postReadSemaphore()
{
    if ( !isReadSemaphoreValid() )
        throw( "PLTwoSemaphores::postReadSemaphore: invalid read semaphore" );

    if ( sem_post( m_rsem ) )
        throw( "PLTwoSemaphores::postReadSemaphore: sem_post failed for read semaphore" );
}

void PLTwoSemaphores::waitReadSemaphore()
{
    if ( !isReadSemaphoreValid() )
        throw( "PLTwoSemaphores::waitReadSemaphore: invalid read semaphore" );

    if ( sem_wait( m_rsem ) )
        throw( "PLTwoSemaphores::waitReadSemaphore: sem_wait failed for read semaphore" );
}

#else //ifdef PL_WXWIDGETS_IPC2

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
#endif //ifdef PL_WXWIDGETS_IPC2
