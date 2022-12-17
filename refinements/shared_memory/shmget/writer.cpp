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

   auto message = std::string("");

   // write to the shared memory
   while (message != "exit") {
      std::cin >> message;
      std::cout<< "Sending: " << message << std::endl;
      memcpy(str, message.c_str(), message.length());
      usleep(1000000);
   }

   // detach from the shared memory
   shmdt(str);

}