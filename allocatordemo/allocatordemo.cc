/**
 * @file allocatordemo.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-09-17
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <bits/types/clock_t.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <ext/array_allocator.h>
#include <ext/bitmap_allocator.h>
#include <ext/debug_allocator.h>
#include <ext/malloc_allocator.h>
#include <ext/mt_allocator.h>
#include <ext/new_allocator.h>
#include <ext/pool_allocator.h>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>

namespace TestAllocator {

void test_list_with_spectial_allocator() {
  std::cout << "test_list_with_spectial_allocator() ......... " << std::endl;

  std::list<std::string, std::allocator<std::string>>              c1;
  std::list<std::string, __gnu_cxx::malloc_allocator<std::string>> c2;
  std::list<std::string, __gnu_cxx::new_allocator<std::string>>    c3;
  std::list<std::string, __gnu_cxx::__pool_alloc<std::string>>     c4;
  std::list<std::string, __gnu_cxx::__mt_alloc<std::string>>       c5;
  std::list<std::string, __gnu_cxx::bitmap_allocator<std::string>> c6;

  int  choice;
  long value;
  std::cout << "select : ";
  std::cin >> choice;

  if (0 != choice) {
    std::cout << "how many elements : ";
    std::cin >> value;
  }

  char    buf[10];
  clock_t time_start = clock();

  for (long i = 0; i < value; i++) {
    try {
      snprintf(buf, 10, "%d", i);

      switch (choice) {
        case 1:
          c1.push_back(std::string(buf));
          break;
        case 2:
          c2.push_back(std::string(buf));
          break;
        case 3:
          c3.push_back(std::string(buf));
          break;
        case 4:
          c4.push_back(std::string(buf));
          break;
        case 5:
          c5.push_back(std::string(buf));
          break;
        case 6:
          c6.push_back(std::string(buf));
          break;
        default:
          break;
      }

    } catch (std::exception &p) {
      std::cout << "i = " << i << " " << p.what() << std::endl;
      abort();
    }
  }
  std::cout << "a lot of push_back(), milli-seconds: " << (clock() - time_start)
            << std::endl;
}

}  // namespace TestAllocator

int main(int argc, char **argv) {
  TestAllocator::test_list_with_spectial_allocator();
}
