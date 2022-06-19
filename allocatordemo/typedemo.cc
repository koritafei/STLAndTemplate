/**
 * @file typedemo.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-09-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <cstddef>
#include <iostream>

template <typename T>
class Bag {
public:
  typedef T           value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef size_t      size_type;

  Bag(T a) : val(a) {
  }

  T getVal() const {
    return val;
  }

private:
  T val;
};

template <typename T>
class Box {
public:
  typedef T           value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef size_t      size_type;

  Box(T val) : val(val) {
  }

  T getVal() const {
    return val;
  }

private:
  T val;
};

template <typename Container>
typename Container::value_type unpack(Container c) {
  return c.getVal();
}

int main(int argc, char** argv) {
  Box<int>  a(100);
  Bag<char> b('a');
  std::cout << unpack(a) << std::endl;
  std::cout << unpack(b) << std::endl;
}