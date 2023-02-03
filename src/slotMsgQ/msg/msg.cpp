#include <msg/msg.hpp>

const msg_t msg_t::invalid_msg = {-1, -1, 0xFFFFFFFF, {0}};
const uint32_t msg_t::MSG_SLOT_SIZE = sizeof(msg_t);

std::ostream& operator<<(std::ostream& os, const msg_t& msg) {
   os << "[" << msg.from << "] -> [" << msg.to << "] : <" << msg.type << ">";
   return os;
}

bool msg_t::isInvalid() {
   return from == -1
         or to == -1
         or type == 0xFFFFFFFF;
}