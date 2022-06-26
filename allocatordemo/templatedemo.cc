#include <iostream>

template <class T>
struct MyIter {
  typedef T value_type;  //声明内嵌类型
  T*        ptr;
  MyIter(T* p = nullptr) : ptr(p) {
  }
  T& operator*() {
    return *ptr;
  }
};

template <class I>
typename I::value_type func(I ite) {
  std::cout << "class version" << std::endl;
  return *ite;
}

template <class I>
I func(I* ite) {
  std::cout << "pointer version" << std::endl;
  return *ite;
}

template <class I>
I func(const I* ite) {
  std::cout << "const pointer version" << std::endl;
  return *ite;
}

int main() {
  MyIter<int> ite(new int(8));
  std::cout << func(ite) << std::endl;
  int* p = new int(52);
  std::cout << func(p) << std::endl;
  const int k = 3;
  std::cout << func(&k) << std::endl;
}