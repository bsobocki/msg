#include <msg/msg.hpp>

std::ostream& operator<<(std::ostream& os, const msg_t& msg) {
   os << "[" << msg.from << "] -> [" << msg.to << "] : <" << msg.type << ">";
   return os;
}

bool msg_t::isInvalid() {
   return from == -1
         or to == -1
         or type == 0xFFFFFFFF;
}