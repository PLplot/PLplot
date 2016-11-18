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

/* Listing 54-3 */

/* pshm_read.c

   Usage: pshm_read shm-name

   Copy the contents of the POSIX shared memory object named in
   'name' to stdout.

   See also pshm_write.c.
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
#include <sys/stat.h>
#include "pshm.h"

int
main(int argc, char *argv[])
{
    int fd;
    char *addr;
    struct stat sb;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
      {
        //usageErr("%s shm-name\n", argv[0]);
	fprintf(stderr, "%s shm-name\n", argv[0]);
	exit(EXIT_FAILURE);
      }

    fd = shm_open(argv[1], O_RDONLY, 0);    /* Open existing object */
    if (fd == -1)
      {
        //errExit("shm_open");
	fprintf(stderr, "shm_open\n");
	exit(EXIT_FAILURE);
      }

    /* Use shared memory object size as length argument for mmap()
       and as number of bytes to write() */

    if (fstat(fd, &sb) == -1)
      {
        //errExit("fstat");
	fprintf(stderr, "fstat\n");
	exit(EXIT_FAILURE);
      }

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
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

    write(STDOUT_FILENO, addr, sb.st_size);
    printf("\n");
    exit(EXIT_SUCCESS);
}
