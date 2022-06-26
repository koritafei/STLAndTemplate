#include <cassert>
#include <iostream>

class Empty {
public:
  void print() {
    std::cout << "I am Empty class" << std::endl;
  }
};

template <typename T>
bool isSame(T const &t1, T const &t2) {
  return &t1 == &t2;
}

class notEbo {
  int   i;
  Empty e;
  // do other things
};

class ebo : public Empty {
  int i;
  // do other things
};

int main(int argc, char **argv) {
  Empty a, b;
  assert(!isSame(a, b));  // 编译通过，a与b的地址不同

  Empty *p = new Empty;
  Empty *q = new Empty;
  assert(!isSame(p, q));  // 编译通过，a与b的地址不同

  delete p, p = nullptr;
  delete q, q = nullptr;

  std::cout << sizeof(Empty) << std::endl;
  std::cout << sizeof(notEbo) << std::endl;
  std::cout << sizeof(ebo) << std::endl;
}