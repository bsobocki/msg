#include <iostream>

#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

int main()
{
   const char *path = "communication";

   int fd = shm_open(path, O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
   if (fd < 0) {
      perror("shm_open");
      return 1;
   }
   
   size_t len = 4096;
   if (ftruncate(fd, len) == -1) {
      perror("ftruncate");
      return 1;
   }

   void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (addr == MAP_FAILED) {
      perror("mmap");
      return 1;
   }

   std::string message = "";

   while (message!="exit") {
      std::cin >> message;
      std::cout<< "Sending: " << message << std::endl;
      memcpy(addr, message.c_str(), message.length());
      usleep(1000000);
   }
   return 0;
}