#include <iostream>
#include <tr1/array>

int main(int argc, char** argv) {
  std::tr1::array<int, 10> a;

  for (int i = 0; i < 10; i++) {
    a[i] = i << 2;
  }

  for (auto i : a) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
}
