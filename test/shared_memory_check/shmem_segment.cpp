#include "shmem_segment.hpp"
#include <unistd.h>
#include <string.h>
#include <fstream>

namespace {

#define failed == -1
#define FULL_SHMEM_SIZE sizeof(std::mutex) + sizeof(accessCounter_t) + shmemSize

}

shmem_segment_t::shmem_segment_t(const char* _shmemKeyFilePath , int size) : shmemSize(size), shmemKeyFilePath(_shmemKeyFilePath) {
   initializeShmemSegment();
}

shmem_segment_t::shmem_segment_t(int size) : shmemSize(size), shmemKeyFilePath(nullptr) {
   initializeShmemSegment();
}

shmem_segment_t::shmem_segment_t(const shmem_segment_t& rhs){
   std::cout<< "copied shmem_segment_t"<<std::endl;
   memory = rhs.getMemory();
   shmem = memory - sizeof(std::mutex) - sizeof(accessCounter_t);
   mutex = reinterpret_cast<std::mutex*>(memory - sizeof(std::mutex));
   counter = reinterpret_cast<accessCounter_t*>(shmem);
   shmemSize = rhs.getMemorySize();
   shmemKeyFilePath = rhs.getShmemKeyFilePath();
   increaseCounter();
}

void shmem_segment_t::initializeShmemSegment() {
   shmem = new int8_t [FULL_SHMEM_SIZE];
   counter = (accessCounter_t*) shmem;
   mutex = (std::mutex*) (shmem + sizeof(accessCounter_t));
   memory = (int8_t*) (shmem + sizeof(accessCounter_t) + sizeof(std::mutex));
   initializeMutex();
   initializeCounter();
   increaseCounter();
   clearMemory();
}

void shmem_segment_t::initializeMutex() {
   new (mutex) std::mutex();
}

void shmem_segment_t::initializeCounter() {
   mutex->lock();
   *counter = 0;
   std::cout << "clearing counter to the value " << *counter << std::endl;
   mutex->unlock();
}

void shmem_segment_t::clearMemory() {
   mutex->lock();
   std::cout << "clearing shared memory segment" << std::endl;
   memset(memory, 0, shmemSize);
   mutex->unlock();
}

void shmem_segment_t::increaseCounter() {
   mutex->lock();
   std::cout<<"increase counter"<<std::endl;
   ++(*counter);
   std::cout<<"current counter value "<<*counter<<std::endl;
   mutex->unlock();
}

void shmem_segment_t::decreaseCounter() {
   mutex->lock();
   std::cout<<"decrease counter"<<std::endl;
   --(*counter);
   std::cout<<"current counter value "<<*counter<<std::endl;
   mutex->unlock();
}

void shmem_segment_t::lock() {
   mutex->lock();
}

void shmem_segment_t::unlock() {
   mutex->unlock();
}

int8_t* shmem_segment_t::getMemory() const {
   return memory;
}

size_t shmem_segment_t::getMemorySize() const {
   return shmemSize;
}

const accessCounter_t shmem_segment_t::getCounterValue() const {
   return *counter;
}

const char* shmem_segment_t::getShmemKeyFilePath() const {
   return shmemKeyFilePath;
}

shmem_segment_t::~shmem_segment_t() {
   decreaseCounter();
   auto count = *counter;

   if (count <= 0) {
      std::cout << "removing shared memory segment" << std::endl;
      delete [] shmem;
   }
}