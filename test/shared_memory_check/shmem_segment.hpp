#ifndef SHMEM_HPP
#define SHMEM_HPP

#include <iostream>
#include <sys/shm.h>
#include <cstring>
#include <mutex>

typedef uint32_t accessCounter_t;

class shmem_segment_t {
public:
   shmem_segment_t(int size);
   shmem_segment_t(const char* _shmemKeyFilePath , int size);
   ~shmem_segment_t();

   int8_t* getMemory();
   size_t getMemorySize();
   const accessCounter_t getCounterValue();

   void lock();
   void unlock();
   void clearMemory();

private:
   void initializeShmemSegment();
   void setShmid(key_t key);
   void attachShmemSegment();
   void attachMutex();
   void attachCounter();
   void attachMemory();
   void initializeMutex();
   void initializeCounter();

   void increaseCounter();
   void decreaseCounter();

   int8_t* shmem;
   std::mutex* mutex;
   accessCounter_t* counter;
   int8_t* memory;

   int shmid;
   size_t shmemSize;
   const char* shmemKeyFilePath;
   bool _hasBeenInitializedBefore;
};

#endif