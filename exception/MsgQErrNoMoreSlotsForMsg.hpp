#include <exception>
#include <string>

class MsgQErrNoMoreSlotsForMsg : public std::exception {
public:
   MsgQErrNoMoreSlotsForMsg(const std::string& msg){
      this->msg = msg;
   }
   virtual ~MsgQErrNoMoreSlotsForMsg() = default;
   virtual const char* what() const noexcept override {
      auto str = "Cannot push " + msg + ". No more slots available in the message queue.";
      return str.c_str();
   }
private:
   std::string msg;
};