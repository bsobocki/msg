#include <iostream>
#include <shared_memory/shmem_segment.hpp>
#include <unistd.h>

int main(){
   shmem_segment_t shmem("/tmp/pid.shmem", sizeof(pid_t));
   
   pid_t* pid = reinterpret_cast<pid_t*>(shmem.getMemory());
   *pid = getpid();

   while(true)
   {
      usleep(500000);
      std::cout << "pid: " << *pid << std::endl;
   }

   std::cout<< "done" << std::endl;

   return 0;
}