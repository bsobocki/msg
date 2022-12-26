#include <iostream>
#include "shm_mutex.hpp"
#include <mutex>
#include <unistd.h>

shm_mutex_t::shm_mutex_t(const char* shmemFilePath): shmem(shmemFilePath, mutexSize + boolSize, &hasBeenInitialized)
{
   mutexMem = reinterpret_cast<std::mutex*>(shmem.getShmemSegment());

   if ( not hasBeenInitialized)
      initialize();
}

void shm_mutex_t::lock(){
   mutexMem->lock();
}

void shm_mutex_t::unlock(){
   mutexMem->unlock();
}

void shm_mutex_t::initialize(){
   std::cout<<"Initializing mutex in "<< getpid() <<std::endl;
   new (mutexMem) std::mutex();
}

/*
NOWY PLAN!!
klasa shmem_t, ktora juz zawiera shm_mutex_t i swoja shmem_segment_t
*/