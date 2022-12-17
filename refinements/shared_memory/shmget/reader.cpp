#include <iostream>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

// path to the file that will be used for generate a key for the shared memory
//const auto keyFile = std::string("/tmp/msglib.key");
constexpr key_t key = 0xc0de;

int main() {
   //key_t key = ftok(keyFile.c_str(), 1);

   // create the shared memory
   int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

   // attach to the shared memory
   char *str = (char *)shmat(shmid, (void *)0, 0);

   while (strcmp(str, "exit") != 0) {
      if (strlen(str)) {
         std::cout<< "Received: " << str << std::endl;
         memcpy(str, "\0", 1);
      }
      usleep(1000000);
   }

   // detach from the shared memory
   shmdt(str);
   shmctl (shmid, IPC_RMID, 0);
}