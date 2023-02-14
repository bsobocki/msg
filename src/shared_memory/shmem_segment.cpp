#include <shmem_segment.hpp>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <ShmemErrInvalidShmemSize.hpp>

namespace {

#define failed == -1
#define FULL_SHMEM_SIZE sizeof(std::mutex) + sizeof(accessCounter_t) + shmemSize

}

shmem_segment_t::shmem_segment_t(const char* _shmemKeyFilePath , size_t size) : shmemSize(size), shmemKeyFilePath(_shmemKeyFilePath) {
   if(access(shmemKeyFilePath, F_OK) == -1){
      std::cout << "shmem_segment_t::shmem_segment_t : creating file " << shmemKeyFilePath << std::endl;
      std::ofstream file(shmemKeyFilePath);
   }
   
   auto key = ftok(shmemKeyFilePath, 'R');
   if (key == -1) {
      perror("shmmem_segment_t::shmem_segment_t : ftok: ");
      perror(shmemKeyFilePath);
      exit(1);
   }

   initializeShmemSegment(key, FULL_SHMEM_SIZE);
}

shmem_segment_t::shmem_segment_t(key_t key, size_t size) : shmemSize(size), shmemKeyFilePath(nullptr) {
   initializeShmemSegment(key, size);
}

void shmem_segment_t::initializeShmemSegment(key_t key, int size) {
   if (not size)
      throw ShmemErrInvalidShmemSize();

   setShmid(key);
   attachShmemSegment();
   attachMutex();
   attachCounter();
   attachMemory();

   if (not _hasBeenInitializedBefore) {
      std::cout<<"shmem_segment_t::shmem_segment_t : shared memory segment has been initialized for the first time."<<std::endl;
      std::cout<<"shmem_segment_t::shmem_segment_t : initialize shared memory segment."<<std::endl;
      initializeMutex();
      initializeCounter();
      clearMemory();
   }

   increaseCounter();
}

void shmem_segment_t::setShmid(key_t key) {
   if ((this->shmid = shmget(key, FULL_SHMEM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) failed) {
      std::cout<<"shmem_segment_t::setShmid : shmget with IPC_EXCL failed."<<std::endl;

      if (errno == EEXIST) {
         std::cout<<"shmem_segment_t::setShmid : shared memory segment already exists."<<std::endl;
         _hasBeenInitializedBefore = true;

         if ((this->shmid = shmget(key, FULL_SHMEM_SIZE, 0666)) failed) {
            perror("shmem_segment_t::setShmid : IPC_CREAT : shmget");
            exit(1);
         }

         std::cout<<"shmem_segment_t::setShmid : assigned an existing shared memory segment with id "<<shmid<<std::endl;
      }
      else if (errno == EINVAL) {
         std::cout<<"shmem_segment_t::setShmid : shared memory segment already exists, but with a different size."<<std::endl;
         std::cout<<"shmem_segment_t::setShmid : get shared memory segment size."<<std::endl;
         // get the size of the shared memory segment
         struct shmid_ds shmid_ds;
         if (shmctl(shmid, IPC_STAT, &shmid_ds) failed) {
            perror("shmem_segment_t::setShmid : IPC_STAT : shmctl");
            exit(1);
         }

         if ((this->shmid = shmget(key, 0, 0666)) failed) {
            perror("shmem_segment_t::setShmid : IPC_CREAT : shmget");
            exit(1);
         }

         if (shmctl(shmid, IPC_RMID, NULL) failed) {
            perror("shmem_segment_t::setShmid : IPC_CREAT : shmctl");
            exit(1);
         }

         std::cout<<"shmem_segment_t::setShmid : create a new shared memory segment with the correct size."<<std::endl;
         if ((this->shmid = shmget(key, FULL_SHMEM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) failed) {
            perror("shmem_segment_t::setShmid : IPC_CREAT : shmget");
            exit(1);
         }
      }
      else if (errno == ENOMEM) {
         std::cout<<"shmem_segment_t::setShmid : not enough memory to create a new shared memory segment."<<std::endl;
         exit(1);
      }
      else if (errno == ENOENT) {
         std::cout<<"shmem_segment_t::setShmid : the directory of the file path does not exist."<<std::endl;
         exit(1);
      }
      else if (errno == ENOSPC) {
         std::cout<<"shmem_segment_t::setShmid : the maximum number of shared memory segments has been reached."<<std::endl;
         exit(1);
      }
      else if (errno == EACCES) {
         std::cout<<"shmem_segment_t::setShmid : the user does not have the permission to create a new shared memory segment."<<std::endl;
         exit(1);
      }
      else if (errno == EEXIST) {
         std::cout<<"shmem_segment_t::setShmid : the shared memory segment already exists."<<std::endl;
         exit(1);
      }
      else if (errno == EIDRM) {
         std::cout<<"shmem_segment_t::setShmid : the shared memory segment has been marked for deletion."<<std::endl;
      }
      else if (errno == ENFILE) {
         std::cout<<"shmem_segment_t::setShmid : the maximum number of shared memory segments has been reached."<<std::endl;
         exit(1);
      }
      else if (errno == ENOENT) {
         std::cout<<"shmem_segment_t::setShmid : the directory of the file path does not exist."<<std::endl;
         exit(1);
      }
      else if (errno == ENOMEM) {
         std::cout<<"shmem_segment_t::setShmid : not enough memory to create a new shared memory segment."<<std::endl;
         exit(1);
      }
      else if (errno == ENOSPC) {
         std::cout<<"shmem_segment_t::setShmid : the maximum number of shared memory segments has been reached."<<std::endl;
         exit(1);
      }
      else if (errno == EPERM) {
         std::cout<<"shmem_segment_t::setShmid : the user does not have the permission to create a new shared memory segment."<<std::endl;
         exit(1);
      }
      else if (errno == EACCES) {
         std::cout<<"shmem_segment_t::setShmid : the user does not have the permission to create a new shared memory segment."<<std::endl;
         exit(1);
      }
      else if (errno == EEXIST) {
         std::cout<<"shmem_segment_t::setShmid : the shared memory segment already exists."<<std::endl;
         exit(1);
      }
      else if (errno == EIDRM) {
         std::cout<<"shmem_segment_t::setShmid : the shared memory segment has been marked for deletion."<<std::endl;
      }
      else if (errno == ENFILE) {
         std::cout<<"shmem_segment_t::setShmid : the maximum number of shared memory segments has been reached."<<std::endl;
         exit(1);
      }
      else if (errno == ENOENT) {
         std::cout<<"shmem_segment_t::setShmid : the directory of the file path does not exist."<<std::endl;
         exit(1);
      }
      else if (errno == ENOMEM) {
         std::cout<<"shmem_segment_t::setShmid : not enough memory to create a new shared memory segment."<<std::endl;
      }
      else {
         perror("shmem_segment_t::setShmid : shmget");
         exit(1);
      }
   }
   else {
      _hasBeenInitializedBefore = false;
      std::cout<<"shmem_segment_t::setShmid : created a new shared memory segment with id "<<shmid<<std::endl;
   }
}

void shmem_segment_t::attachShmemSegment() {
   if ((shmem = (int8_t*)shmat(shmid, 0, 0)) == reinterpret_cast<int8_t*>(-1)) {
         perror("shmem_segment_t::setShmid : shmat");
         exit(1);
   }
}

void shmem_segment_t::attachMutex() {
   this->mutex = reinterpret_cast<std::mutex*>(shmem);
}

void shmem_segment_t::attachCounter() {
   this->counter = reinterpret_cast<accessCounter_t*>(shmem + sizeof(std::mutex)); 
}

void shmem_segment_t::attachMemory() {
   this->memory = reinterpret_cast<int8_t*>(shmem + sizeof(std::mutex) + sizeof(accessCounter_t));
}

void shmem_segment_t::initializeMutex() {
   new (mutex) std::mutex();
}

void shmem_segment_t::initializeCounter() {
   mutex->lock();
   std::cout << "clearing counter" << std::endl;
   *counter = 0;
   mutex->unlock();
}

void shmem_segment_t::clearMemory() {
   mutex->lock();
   std::cout << "clearing shared memory segment" << std::endl;
   memset(memory, 0, shmemSize);
   mutex->unlock();
}

void shmem_segment_t::increaseCounter() {
   mutex->lock();
   std::cout<<"increase counter"<<std::endl;
   ++(*counter);
   std::cout<<"current counter value "<<*counter<<std::endl;
   mutex->unlock();
}

void shmem_segment_t::decreaseCounter() {
   mutex->lock();
   std::cout<<"decrease counter"<<std::endl;
   --(*counter);
   std::cout<<"current counter value "<<*counter<<std::endl;
   mutex->unlock();
}

void shmem_segment_t::lock() {
   mutex->lock();
}

void shmem_segment_t::unlock() {
   mutex->unlock();
}

int8_t* shmem_segment_t::getMemory() const {
   return memory;
}

size_t shmem_segment_t::getMemorySize() const {
   return shmemSize;
}

const accessCounter_t shmem_segment_t::getCounterValue() const {
   return *counter;
}

const char* shmem_segment_t::getShmemKeyFilePath() const {
   return shmemKeyFilePath;
}

shmem_segment_t::~shmem_segment_t() {
   decreaseCounter();
   auto count = *counter;

   shmdt(shmem);

   if (count <= 0) {
      std::cout << "removing shared memory segment" << std::endl;
      shmctl(shmid, IPC_RMID, 0);
      if (shmemKeyFilePath) {
         std::cout << "removing shared memory key file" << std::endl;
         std::remove(shmemKeyFilePath);
      }
   }
}