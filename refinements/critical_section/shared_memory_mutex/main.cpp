#include <iostream>
#include "shmem.hpp"
#include <unistd.h>
#include <ctime>

const size_t shmemSize = sizeof(pid_t) + sizeof(int32_t);

void printData(void* data, size_t size) {
   auto pid = reinterpret_cast<pid_t*>(data);
   auto num = reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(data) + sizeof(pid_t));
   std::cout << "pid: " << *pid << ", num: " << *num << std::endl;
}

int main() {
   srand(time(0));
   shmem_t shmem(shmem_t::DEFAULT_FILE_PATH, shmemSize);
   for(int i = 0; i < 35; ++i){
      shmem.useShmem(printData);
      usleep(700000 + rand()%1000000);
   }
}