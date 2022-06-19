/**
 * @file hashfunc.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-09-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <boost/functional/hash.hpp>
#include <iostream>

template <typename T>
inline void hash_combine(size_t &seed, const T &val) {
  // 利用0x9e3779b9做hash分散
  seed ^= boost::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T>
inline void hash_val(size_t &seed, const T &val) {
  boost::hash_combine(seed, val);
}

template <typename T, typename... Types>
inline void hash_val(size_t &seed, const T &val, const Types &...args) {
  hash_combine(seed, val);
  hash_val(seed, args...);
}

template <typename... Types>
inline size_t hash_val(const Types &...args) {
  size_t seed = 0;
  hash_val(seed, args...);
  return seed;
}

class Customer {
public:
  std::string fname;
  std::string lname;
  int         no;

  Customer(std::string fname, std::string lname, int no)
      : fname(fname), lname(lname), no(no) {
  }
};

class CustomerHash {
public:
  size_t operator()(const Customer &c) const {
    return hash_val(c.fname, c.lname, c.no);
  }
};

int main(int argc, char **argv) {
  CustomerHash hh;
  std::cout << hh(Customer("Ace", "Hou", 1L)) % 11 << std::endl;
  std::cout << hh(Customer("Sabri", "Hou", 2L)) % 11 << std::endl;
  std::cout << hh(Customer("Stacy", "Chen", 3L)) % 11 << std::endl;
  std::cout << hh(Customer("Mike", "Tseng", 4L)) % 11 << std::endl;
  std::cout << hh(Customer("Paili", "Chen", 5L)) % 11 << std::endl;
  std::cout << hh(Customer("Light", "Shiau", 6L)) % 11 << std::endl;
}