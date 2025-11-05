/*
 * hello world in in C++
 * compile: g++ hello.cpp -o hello.exe
 * run: hello.exe
 */
#include <iostream>

//C is a subset of C++,  that means C code is allowed in C++ programs
#include <stdio.h> // this includes C's stdio library header
  
  
int main()
{
  std::cout << "hello, world" << "\n";  // this is to output string to standard output  
  std::cout << "Hello world" << std::endl;  // add newline at the end
  std::cout << "Hello, world!" << std::ends;  // add newline at the end and flush the stream
  
  using namespace std;  // this add scope std to program, then cont represents std::cout
  cout << "Hello namespace" << endl; 
  
  printf("Hello World!\n");  // this is formatted printing function from C's stdio library
  
  return 0;
}
