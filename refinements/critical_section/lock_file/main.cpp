#include <iostream>

const auto lockFile = std::string("/tmp/msglib.lock");

int main() {
   std::cout << "Hello, World!" << std::endl;
   return 0;
}