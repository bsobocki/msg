#include <iostream>
#include <stdint.h>
#include <shared_memory/shmem_segment.hpp>
#include <msg/msg.hpp>

struct msgQ_t {
public:
   msgQ_t(const char* path, size_t _capacity);

   void push(const msg_t & msg);
   const msg_t pop();
   bool empty();
   bool full();
   const size_t getSize();
   const size_t getCapacity();

private:
   size_t size;
   size_t capacity;
   shmem_segment_t shmem;
   size_t top;
   msg_t* msgs;
};