#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <shared_memory/shmem_segment.hpp>

const char* path = "/tmp/pid.shmem";

std::string isAlive(pid_t pid) {
   if (kill(pid, 0) == 0)
      return " is alive";
   return " is dead";
}

int main() {
   shmem_segment_t shmem(path, sizeof(pid_t));
   
   const pid_t* pid = reinterpret_cast<pid_t*>(shmem.getMemory());
   
   while(true)
   {
      usleep(500000);
      std::cout << "pid: " << *pid << isAlive(*pid) << std::endl;
   }
   
}