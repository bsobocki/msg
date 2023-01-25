#include <iostream>
#include <cstring>

struct A {
  short a;
  char b;
  char c[1]; // if we used char* then the address stored in this pointer would be the sizeof(char*) bytes of data
             // so if we used char* then we would try to use this data as an adress, so it is the wrong address
             // but if we used char[1] then we can use variable c as the pointer to the address of the first element of the data
};

struct B {
  short a;
  char b;
  char c[30];
};

int main() {
  // quick check
  B b{15,'a', "Hello World! Welcome to C++!"};
  A a;
  memcpy(&a, &b, sizeof(A));
  // copied data will be readed as a struct A
  std::cout << a.a << std::endl;  // output: 15
  std::cout << a.b << std::endl;  // output: a
  std::cout << a.c << std::endl;  // output: H
  std::cout << std::endl;

  char str[29] = "Hello World! Welcome to C++!";
  // copied data will be still readed as a struct A
  memcpy(&a, str, sizeof(A));
  std::cout << a.a << std::endl;  // output: 25928
  std::cout << a.b << std::endl;  // output: l
  std::cout << a.c << std::endl;  // output: l
  std::cout << std::endl;

  // the data will be readed as a struct A
  // pointers should be the same
  A* a_ptr = reinterpret_cast<A*>(&b);
  printf("b ptr: %p\n", (void*)&b);
  printf("a_ptr ptr: %p\n", (void*)a_ptr);
  printf("b.a ptr: %p\n", (void*)&(b.a));
  printf("a_ptr->a ptr: %p\n", (void*)&(a_ptr->a));
  printf("b.b ptr: %p\n", (void*)&(b.b));
  printf("a_ptr->b ptr: %p\n", (void*)&(a_ptr->b));
  printf("b.c ptr: %p\n", (void*)(b.c));
  printf("a_ptr->c ptr: %p\n", (void*)a_ptr->c);
  std::cout << std::endl;

  std::cout << a_ptr->a << std::endl;  // output: 15
  std::cout << a_ptr->b << std::endl;  // output: a
  // if we used char* in struct A, then we will get a segmentation fault here
  std::cout << a_ptr->c << std::endl;  // output: Hello World! Welcome to C++!
  std::cout << std::endl;
  
  // the data will be readed as a struct A
  A* aa = reinterpret_cast<A*>(str);

  std::cout << aa->a << std::endl;  // output: 25928
  std::cout << aa->b << std::endl;  // output: l
  // if we used char* in struct A, then we will get a segmentation fault here
  std::cout << aa->c << std::endl;  // output: lo World! Welcome to C++
  return 0;
}