#pragma once

#include <iostream>
#include <stdint.h>
#include <shmem_segment.hpp>
#include <msg/msg.hpp>
#include <mutex>

struct msgQ_t {
public:
   msgQ_t(const char* path, size_t _capacity);

   void push(const msg_t & msg);
   void push(const msg_t && msg);
   const msg_t pop();
   const msg_t peek();
   bool empty();
   bool full();
   const size_t getSize();
   const size_t getCapacity();

private:
   void moveTop();

   size_t size;
   size_t top;
   size_t capacity;
   shmem_segment_t shmem;
   msg_t* msgs;
};