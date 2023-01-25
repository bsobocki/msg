#include <msg/msg.hpp>

std::ostream& operator<<(std::ostream& os, const msg_t& msg) {
   os << "[" << msg.from << "] -> [" << msg.to << "] : <" << msg.type << ">";
   return os;
}