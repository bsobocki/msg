#include "shm_mutex.hpp"
#include "shmem_segment.hpp"

const char* shmemFilePath = "communication.shmem";

class shmem_t {
public:
   shmem_t(const char* path, size_t size);

private:
   shmem_segment_t shmem;
   shm_mutex_t mutex;
};