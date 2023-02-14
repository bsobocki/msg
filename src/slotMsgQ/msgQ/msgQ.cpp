#include <msgQ/msgQ.hpp>
#include <MsgQErrNoMoreSlotsForMsg.hpp>

msgQ_t::msgQ_t(shmem_segment_t& _shmem, size_t _capacity):
   size(0),
   top(0),
   capacity(_capacity),
   shmem(_shmem) {
   msgs = reinterpret_cast<msg_t*>(shmem.getMemory());
}

msgQ_t::msgQ_t(const char* path, size_t _capacity):
   size(0),
   top(0),
   capacity(_capacity),
   shmem(path, _capacity * msg_t::MSG_SLOT_SIZE) {
   msgs = reinterpret_cast<msg_t*>(shmem.getMemory());
}

void msgQ_t::push(const msg_t & msg) {
   if (size < capacity) {
      size_t firstFreeIndex = top + size;
      if (firstFreeIndex >= capacity)
         firstFreeIndex -= capacity;
      memcpy(msgs + firstFreeIndex, &msg, msg_t::MSG_SLOT_SIZE);
      size++;
   }
   else
      throw MsgQErrNoMoreSlotsForMsg(msgToStr(msg));
}

void msgQ_t::push(const msg_t && msg){
   push(msg);
}

const msg_t msgQ_t::pop() {
   if (not empty()) {
      msg_t msg = msgs[top];
      moveTop();
      --size;
      return msg;
   }
   else {
      std::cout << "msgQ is empty! Cannot pop message!" << std::endl;
      return msg_t::invalid_msg;
   }
}

void msgQ_t::moveTop() {
   top++;
   if (top == capacity) top = 0;
}

const msg_t msgQ_t::peek() {
   if (not empty())
      return msgs[top];
   else {
      std::cout << "msgQ is empty! Cannot peek message!" << std::endl;
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