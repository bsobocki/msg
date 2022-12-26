#include "critical_section.hpp"
#include "shmem.hpp"

const char* shmemFilePath = "communication.shmem";

int main() {
   shm_mutex_t mutex;

   shmem_t shmem(shmemFilePath, 1024);

   mutex.lock();
   mutex.unlock();
}