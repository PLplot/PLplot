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
    size_t       readLocation;
    size_t       writeLocation;
    size_t       viewerOpenFlag;
    size_t       locateModeFlag;
    size_t       completeFlag;
    PLGraphicsIn graphicsIn;
    TextSizeInfo textSizeInfo;
};

#ifdef PL_WXWIDGETS_IPC2
#define PL_SHARED_ARRAY_SIZE    1024 * 1024
// Shared memory buffer type
struct shmbuf
{
#ifdef PL_HAVE_UNNAMED_POSIX_SEMAPHORES
    sem_t  wsem;                      // Writer semaphore
    sem_t  rsem;                      // Reader semaphore
#endif
    size_t cnt;                       // Number of bytes used in 'buf'
    size_t total;                     // Total number of bytes to be transferred
    char   buf[PL_SHARED_ARRAY_SIZE]; // Data being transferred
};

class PLTwoSemaphores
{
public:
    PLTwoSemaphores( sem_t *wsem, sem_t *rsem, bool mustExist );
    ~PLTwoSemaphores();
    void postWriteSemaphore();
    void waitWriteSemaphore();
    void postReadSemaphore();
    void waitReadSemaphore();

private:
    sem_t * m_wsem;
    sem_t * m_rsem;
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
    char *getBuffer() { return ( (shmbuf *) m_buffer )->buf; }
#ifdef PL_HAVE_UNNAMED_POSIX_SEMAPHORES
    sem_t *getWriteSemaphore() { return &( ( (shmbuf *) m_buffer )->wsem ); }
    sem_t *getReadSemaphore() { return &( ( (shmbuf *) m_buffer )->rsem ); }
#endif
    size_t *getBytesUsed() { return &( ( (shmbuf *) m_buffer )->cnt ); }
    size_t *getBytesTotal() { return &( ( (shmbuf *) m_buffer )->total ); }
    size_t getSize() { return PL_SHARED_ARRAY_SIZE; }
    void sendBytes( PLTwoSemaphores two_semaphores, size_t nbytes, const void * bytes );
    void receiveBytes( PLTwoSemaphores two_semaphores, size_t *p_nbytes, void * bytes );
#else // #ifdef PL_WXWIDGETS_IPC2
    char *getBuffer() { return (char *) m_buffer; }
    size_t getSize() { return m_size; }
#endif // #ifdef PL_WXWIDGETS_IPC2
private:
#ifdef WIN32
    HANDLE m_mapFile;
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
