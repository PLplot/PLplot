/*************************************************************************\
*                  Copyright (C) Alan W. Irwin, 2016.                     *
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 54-2 */

/* pshm_write.c

   Usage: pshm_write shm-name string

   Copy 'string' into the POSIX shared memory object named in 'shm-name'.

   See also pshm_read.c.
*/
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "pshm.h"

int
main(int argc, char *argv[])
{
    int fd;
    size_t len;                 /* Size of shared memory object */
    char *addr;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
      {
        //usageErr("%s shm-name string\n", argv[0]);
	fprintf(stderr, "%s shm-name string\n", argv[0]);
	exit(EXIT_FAILURE);
      }

    fd = shm_open(argv[1], O_RDWR, 0);      /* Open existing object */
    if (fd == -1)
      {
        //errExit("shm_open");
	fprintf(stderr, "shm-open\n");
	exit(EXIT_FAILURE);
      }

    len = strlen(argv[2]);
    if (ftruncate(fd, len) == -1)           /* Resize object to hold string */
      {
        //errExit("ftruncate");
	fprintf(stderr, "ftruncate\n");
	exit(EXIT_FAILURE);
      }
    printf("Resized to %ld bytes\n", (long) len);

    addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
      {
        //errExit("mmap");
	fprintf(stderr, "mmap\n");
	exit(EXIT_FAILURE);
      }

    if (close(fd) == -1)                    /* 'fd' is no longer needed */
      {
        //errExit("close");
	fprintf(stderr, "close\n");
	exit(EXIT_FAILURE);
      }

    printf("copying %ld bytes\n", (long) len);
    memcpy(addr, argv[2], len);             /* Copy string to shared memory */
    exit(EXIT_SUCCESS);
}
