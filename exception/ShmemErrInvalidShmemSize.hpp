#include <exception>
#include <string>

class ShmemErrInvalidShmemSize : public std::exception {
public:
   ShmemErrInvalidShmemSize(){
      this->msg = msg;
   }
   virtual ~ShmemErrInvalidShmemSize() = default;
   virtual const char* what() const noexcept override {
      return "Cannot create message queue. Invalid shared memory size.";
   }
private:
   std::string msg;
};