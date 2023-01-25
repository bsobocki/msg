#include "shmem.hpp"

shmem_t::shmem_t(const char* path, size_t size) 
: shmem(path, size + sizeof(accessCounter_t)) {
}

void shmem_t::write(const char* data, size_t size) {
   if (size > shmem.getMemorySize())
      size = shmem.getMemorySize();

   shmem.lock();
   memcpy(shmem.getMemory(), data, size);
   shmem.unlock();
}

void shmem_t::read(char* data, size_t size) {
   if (size > shmem.getMemorySize())
      size = shmem.getMemorySize();
   
   shmem.lock();
   memcpy(data, shmem.getMemory(), size);
   shmem.unlock();
}

void shmem_t::useShmem(void (*foo)(void*, size_t)) {
   shmem.lock();
   foo(shmem.getMemory(), shmem.getMemorySize());
   shmem.unlock();
}