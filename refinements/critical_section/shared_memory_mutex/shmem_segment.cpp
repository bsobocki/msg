#include "shmem_segment.hpp"

shmem_segment_t::shmem_segment_t(const char* shmemFilePath , int size, bool* hasBeenInitializedBefore) : shmemSize(size) {
   auto key = ftok(shmemFilePath, 'R');
   if (key == -1) {
      perror("shm_mem_t :: ftok");
      exit(1);
   }
   
   setShmid(key, hasBeenInitializedBefore);
   setShmemSegment();
   clearMem();
}

shmem_segment_t::shmem_segment_t(key_t key, int size, bool* hasBeenInitializedBefore) : shmemSize(size) {
   setShmid(key, hasBeenInitializedBefore);
   setShmemSegment();
   clearMem();
}

void shmem_segment_t::setShmid(key_t key, bool* hasBeenInitializedBefore) {
   if (hasBeenInitializedBefore != nullptr) {
      if ((this->shmid = shmget(key, shmemSize, IPC_EXCL | 0666)) == -1) {
         if (errno == EEXIST)
            *hasBeenInitializedBefore = true;
      }
      else
         *hasBeenInitializedBefore = false;
   }

   if ((this->shmid = shmget(key, shmemSize, IPC_CREAT | 0666)) == -1) {
      perror("shm_mem_t :: shmget");
      exit(1);
   }
}

void shmem_segment_t::setShmemSegment() {
   if ((mem = (int8_t*)shmat(shmid, 0, 0)) == reinterpret_cast<int8_t*>(-1)) {
         perror("shm_mem_t :: shmat");
         exit(1);
   }
}

int8_t* shmem_segment_t::getShmemSegment() {
   return mem;
}

void shmem_segment_t::clearMem() {
   memset(mem, 0, shmemSize);
}

shmem_segment_t::~shmem_segment_t() {
   shmdt(mem);
   shmctl(shmid, IPC_RMID, 0);
}