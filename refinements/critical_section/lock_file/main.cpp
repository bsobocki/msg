#include <iostream>

const auto lockFile = std::string("/tmp/msglib.lock");

void lock()
{
   std::ofstream lockF(lockFile);
}

int main() {
   std::cout << "Hello, World!" << std::endl;
   return 0;
}