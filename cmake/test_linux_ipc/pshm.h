#ifndef __PSHM_H__
#define __PSHM_H__

#define BUF_SIZE 1024
struct shmbuf {       // Buffer in shared memory
  sem_t wsem;         // Writer semaphore
  sem_t rsem;         // Reader semaphore
  int cnt;            // Number of bytes used in 'buf'
  char buf[BUF_SIZE]; // Data being transferred
};
#endif
