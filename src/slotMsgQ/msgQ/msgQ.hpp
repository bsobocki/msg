#include <iostream>
#include <stdint.h>
#include <shared_memory/shmem_segment.hpp>
#include <msg/msg.hpp>

struct msgQ_t {
public:
   msgQ_t(const char* path, size_t _capacity) : capacity(_capacity), shmem(path, _capacity*MSG_SLOT_SIZE) {
      msgs = reinterpret_cast<msg_t*>(shmem.getMemory());
      capacity = _capacity;
      size = 0;
   }

   void push(msg_t msg) {
      if (size < capacity) {
         msgs[size] = msg;
         ++size;
      }
      else
         std::cout << "msgQ is full! Cannot push message:" << msg << std::endl;
   }

   msg_t pop() {
      if (size > 0) {
         msg_t msg = msgs[0];
         for (int i = 0; i < size-1; ++i) {
            msgs[i] = msgs[i+1];
         }
         --size;
         return msg;
      }
   }

   bool empty() {
      return size == 0;
   }

   bool full() {
      return size == capacity;
   }

   size_t getSize() {
      return size;
   }

   size_t getCapacity() {
      return capacity;
   }

private:
   int size;
   int capacity;
   shmem_segment_t shmem;
   msg_t* msgs;
};