// Copyright (C) 2015-2017 Phil Rosenberg
// Copyright (C) 2017 Alan W. Irwin
// Copyright (C) 2008 Werner Smekal

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

#include <wx/font.h>
#include "wxPLplot_nanosec.h"

//data transmission codes
const unsigned char transmissionRegular          = 0;
const unsigned char transmissionSkipFileEnd      = 1;
const unsigned char transmissionEndOfPage        = 2;
const unsigned char transmissionBeginPage        = 3;
const unsigned char transmissionLocate           = 4;
const unsigned char transmissionPartial          = 5;
const unsigned char transmissionComplete         = 6;
const unsigned char transmissionRequestTextSize  = 7;
const unsigned char transmissionEndOfPageNoPause = 8;
const unsigned char transmissionClose            = 9;
#ifdef PL_WXWIDGETS_IPC2
const unsigned char transmissionFlush = 10;
#endif

#define TEXTSIZEINFO_MAX_LENGTH    500

struct TextSizeInfo
{
    long         width;
    long         height;
    long         depth;
    long         leading;
    wxFontFamily family;
    int          style;
    int          weight;
    int          pt;
    bool         underlined;
    wchar_t      text[TEXTSIZEINFO_MAX_LENGTH + 1];
    bool         written;
};

struct MemoryMapHeader
{
    size_t        viewerOpenFlag;
    size_t        locateModeFlag;
    size_t        completeFlag;
#ifdef PL_WXWIDGETS_IPC2
    size_t        plbufAmountToTransmit;
    unsigned char transmissionType;
#else
    size_t        readLocation;
    size_t        writeLocation;
#endif
    PLGraphicsIn  graphicsIn;
    TextSizeInfo  textSizeInfo;
};

#ifdef PL_WXWIDGETS_IPC2
// I plan to reduce this size in the future if that reduction
// does not significantly affect efficiency.
#define PL_SHARED_ARRAY_SIZE    1024 * 1024

// In the three-semaphores method of IPC, the shared memory area must
// correspond to this shmbuf struct which contains some control data
// explicitly used for the communication, e.g., at least the total
// number of bytes of data to be transferred, and limited size
// data areas to be transferred under three-semaphore control.
struct shmbuf
{
#ifdef PL_HAVE_UNNAMED_POSIX_SEMAPHORES
    // control data
    sem_t           wsem;             // Writer semaphore
    sem_t           rsem;             // Reader semaphore
    sem_t           tsem;             // Transmit semaphore
#endif
    size_t          nbytes;           // Total number of data bytes to be transferred
    // header data to be transferred under three-semaphore control.
    MemoryMapHeader header;
    // plbuf data to be transferred under three-semaphore control.
    char            data[PL_SHARED_ARRAY_SIZE];
};

class PLThreeSemaphores
{
public:
    // Default constructor: Initialize m_wsem, m_rsem, and m_tsem to
    // NULL to mark those as invalid semaphore locations.
    PLThreeSemaphores();
    // Update the m_wsem, m_rsem, and m_tsem locations and
    // conditionally (when mustExist is false) initialize those
    // locations as semaphores.
    void initializeToValid( sem_t *wsem, sem_t *rsem, sem_t *tsem, bool mustExist );
    // call initializeToInvalid.
    ~PLThreeSemaphores();
    // If the m_wsem, m_rsem, and m_tsem locations are non-NULL
    // destroy those semaphores.  Also, unconditionally set
    // m_wsem, m_rsem, and m_tsem to NULL to mark those as invalid
    // semaphore locations.
    void initializeToInvalid();
    bool isWriteSemaphoreValid();
    bool isReadSemaphoreValid();
    bool isTransmitSemaphoreValid();
    // Return true if all semaphores are valid.
    // Return false if all semaphores are invalid.
    // Throw an exception otherwise.
    bool areSemaphoresValid();
    // Check whether write and read semaphores are valid and blocked.
    bool areWriteReadSemaphoresBlocked();
    // Get value of Write semaphore.
    int getValueWriteSemaphore();
    // Get value of Read semaphore.
    int getValueReadSemaphore();
    void postWriteSemaphore();
    void waitWriteSemaphore();
    void postReadSemaphore();
    void waitReadSemaphore();
    void postTransmitSemaphore();
    void waitTransmitSemaphore();
private:
    // Attempts to test semaphore initialization validity using
    // sem_getvalue on Linux proved fruitless since as far as I can tell
    // with gdb that function always returns zero, i.e., always signals
    // success, and returns a 0 or 1 value argument ___so long as its
    // sem_t * argument points to _any_ accessible memory area that is
    // cast to sem_t *__!  So here is the alternative plan we are using:
    // m_wsem, m_rsem, and m_tsem should always be NULL unless the non-NULL
    // locations they point to are properly initialized semaphores.
    sem_t *m_wsem;
    sem_t *m_rsem;
    sem_t *m_tsem;
};

#endif //#ifdef PL_WXWIDGETS_IPC2

const PLINT plMemoryMapReservedSpace = sizeof ( MemoryMapHeader );

class PLMemoryMap
{
public:
    PLMemoryMap();
    PLMemoryMap( const char *name, PLINT size, bool mustExist, bool mustNotExist );
    void create( const char *name, PLINT size, bool mustExist, bool mustNotExist );
    void close();
    ~PLMemoryMap();
    bool isValid() { return m_buffer != NULL; }
#ifdef PL_WXWIDGETS_IPC2
    char *getBuffer() { return ( (shmbuf *) m_buffer )->data; }
    MemoryMapHeader *getHeader() { return &( ( (shmbuf *) m_buffer )->header ); }
#ifdef PL_HAVE_UNNAMED_POSIX_SEMAPHORES
    sem_t *getWriteSemaphore() { return &( ( (shmbuf *) m_buffer )->wsem ); }
    sem_t *getReadSemaphore() { return &( ( (shmbuf *) m_buffer )->rsem ); }
    sem_t *getTransmitSemaphore() { return &( ( (shmbuf *) m_buffer )->tsem ); }
#endif
    size_t *getTotalDataBytes() { return &( ( (shmbuf *) m_buffer )->nbytes ); }
    size_t getSize() { return PL_SHARED_ARRAY_SIZE; }
    void transmitBytes( bool ifHeader, const void *src, size_t n );
    void receiveBytes( bool ifHeader, void *dest, size_t n );
    void initializeSemaphoresToValid( sem_t *wsem, sem_t *rsem, sem_t *tsem, bool mustExist ) { m_threeSemaphores.initializeToValid( wsem, rsem, tsem, mustExist ); }
#else // #ifdef PL_WXWIDGETS_IPC2
    char *getBuffer() { return (char *) m_buffer; }
    size_t getSize() { return m_size; }
#endif // #ifdef PL_WXWIDGETS_IPC2
private:
#ifdef WIN32
    HANDLE m_mapFile;
#elif defined ( PL_WXWIDGETS_IPC2 )
    int m_mapFile;
    char * m_name;
    // instantiate m_threeSemaphores private object (with default
    // constructor) when PLMemoryMap is instantiated.
    PLThreeSemaphores m_threeSemaphores;
#else
    int m_mapFile;
    char * m_name;
#endif
    // Size of shared memory buffer
    size_t m_size;
    void   *m_buffer;
};

#ifndef PL_WXWIDGETS_IPC2

class PLNamedMutex
{
public:
    PLNamedMutex();
    ~PLNamedMutex();
    PLNamedMutex( const char *name, bool aquireOnCreate = false );
    void create( const char *name, bool aquireOnCreate = false );
    void clear();
    void aquire();
    bool aquire( unsigned long millisecs );
    bool aquireNoWait();
    void release();
    bool isValid();
private:
    bool   m_haveLock;
#ifdef WIN32
    HANDLE m_mutex;
#else
    sem_t  * m_mutex;
    char   m_mutexName[251];
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
    PLNamedMutexLocker & operator =( const PLNamedMutex & );
};
#endif //ifndef PL_WXWIDGETS_IPC2

#endif
