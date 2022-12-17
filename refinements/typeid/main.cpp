#include <iostream>
#include <memory>
#include <typeindex>


template <typename T>
class msg {
public:
   msg(T data) {
      this->data = std::make_shared<T>(data);
   }

   auto getId() {
      return std::type_index(typeid(T)).hash_code();
   }

   auto getData() {
      return data;
   }

private:
   msg_data_t data;
};

struct FirstStruct {
   int a;
   int b;
};

struct SecondStruct {
   std::string str;
};

struct ThirdStruct {
   double d;
};

template <typename T>
constexpr size_t getTypeId() {
   return typeid(T).hash_code();
}

template <typename T>
void printMsg(msg<T>& message) {
   auto data = message.getData();

   switch(message.getId()) {
      case getTypeId<FirstStruct>():
         std::cout << "FirstStruct: "<< data->a << " " << data->b << std::endl;
         break;
      case getTypeId<SecondStruct>():
         std::cout << "SecondStruct: \"" << data->str << "\"" << std::endl;
         break;
      case getTypeId<ThirdStruct>():
         std::cout << "ThirdStruct: " << data->d << std::endl;
         break;
      default:
         std::cout << "Unknown" << std::endl;
         break;
   }
}

int main() {
   msg<FirstStruct> mesg(FirstStruct{1, 2});
   printMsg(mesg);

   msg<SecondStruct> mesg1(SecondStruct{"stringus"});
   printMsg(mesg1);

   msg<ThirdStruct> mesg2(ThirdStruct{0.5});
   printMsg(mesg2);

   return 0;
}