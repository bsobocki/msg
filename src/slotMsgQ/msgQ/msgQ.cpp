#include <msgQ/msgQ.hpp>

msgQ_t::msgQ_t(const char* path, size_t _capacity):
   size(0),
   beg(0),
   capacity(_capacity),
   shmem(path, _capacity*MSG_SLOT_SIZE) {
   msgs = reinterpret_cast<msg_t*>(shmem.getMemory());
}

void msgQ_t::push(const msg_t & msg) {
   if (size < capacity) {
      size_t ind = top + size;
      if (ind >= capacity) ind -= capacity;
      memcpy(msgs + ind, &msg, MSG_SLOT_SIZE);
      ++size;
   }
   else
      std::cout << "msgQ is full! Cannot push message:" << msg << std::endl;
}

const msg_t msgQ_t::pop() {
   if (not empty()) {
      msg_t msg = msgs[top++];
      if (top == capacity) top = 0;
      --size;
      return msg;
   }
   else {
      std::cout << "msgQ is empty! Cannot pop message!" << std::endl;
      return msg_t::invalid_msg;
   }
}

bool msgQ_t::empty() {
   return size == 0;
}

bool msgQ_t::full() {
   return size == capacity;
}

const size_t msgQ_t::getSize() {
   return size;
}

const size_t msgQ_t::getCapacity() {
   return capacity;
}