#include "critical_section.hpp"
#include "shmem.hpp"

const char* shmemFilePath = "communication.shmem";

int main() {
   shm_mutex_t mutex;
   mutex.lock();
   mutex.unlock();
}