#include <iostream>

#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

using namespace std;

int main()
{
   const char *path = "communication";

   int fd = shm_open(path, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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
   if (addr == MAP_FAILED || addr == NULL) {
      perror("mmap");
      return 1;
   }

   memcpy(addr, "\0", 1);

   while (true) {
      if (strlen((char *)addr)) {
         std::cout<< "Received: " << (char *)addr << std::endl;
         memcpy(addr, "\0", 1);
         break;
      }
      usleep(1000000);
   }

   shm_unlink(path);

   return 0;
}