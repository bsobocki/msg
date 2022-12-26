#ifndef SHMEM_HPP
#define SHMEM_HPP

#include <iostream>
#include <sys/shm.h>
#include <cstring>

//create a shared memory segment
class shmem_segment_t {
public:
   shmem_segment_t(key_t key, int size, bool* hasBeenInitializedBefore = nullptr);
   shmem_segment_t(const char* shmemFilePath , int size, bool* hasBeenInitializedBefore = nullptr);
   ~shmem_segment_t();
   int8_t* getShmemSegment();
   void clearMem();

private:
   void setShmid(key_t key, bool* hasBeenInitializedBefore);
   void setShmemSegment();

   int shmid;
   size_t shmemSize;
   int8_t* mem;
};

#endif