#include "shmem_segment.hpp"

typedef uint32_t accessCounter_t;

class shmem_t {
public:
   shmem_t(const char* path, size_t size);
   ~shmem_t() = default;
   void write(const char* data, size_t size);
   void read(char* data, size_t size);
   void useShmem(void (*transform)(void*, size_t));
   const accessCounter_t getCounterValue();

private:
   shmem_segment_t shmem;

public:
   static constexpr const char* DEFAULT_FILE_PATH = "/tmp/communication.shmem";
   static const size_t DEFAULT_SIZE = 1024;
};