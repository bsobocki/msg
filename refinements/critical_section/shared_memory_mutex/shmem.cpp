#include "shmem.hpp"

shmem_t::shmem_t(const char* path, size_t size) 
: shmem(path, size), mutex() {
}

void shmem_t::write(const char* data, size_t size) {
   mutex.lock();
   memcpy(shmem.getShmemSegment(), data, size);
   mutex.unlock();
}

void shmem_t::read(char* data, size_t size) {
   mutex.lock();
   memcpy(data, shmem.getShmemSegment(), size);
   mutex.unlock();
}

void shmem_t::useShmem(void (*foo)(void*, size_t)) {
   mutex.lock();
   foo(shmem.getShmemSegment(), shmem.getShmemSize());
   mutex.unlock();
}