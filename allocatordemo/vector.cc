
#include <cstddef>
template <class T, class Alloc = alloc>
class Vector {
public:
  typedef T           value_type;  // 用于返回值类型
  typedef value_type* iterator;
  typedef value_type& reference;
  typedef size_t      size_type;

  iterator begin() {
    return start;
  }

  iterator end() {
    return finish;
  }

  size_type size() const {
    return size_type(end() - begin());
  }

  size_type capacity() const {
    return size_type(end_of_storage - begin());
  }

  bool empty() const {
    return begin() == end();
  }

  reference operator[](size_type n) {
    return *(begin() + n);
  }

  reference front() {
    return *begin();
  }

  reference back() {
    return *(end() - 1);
  }

protected:
  iterator start;
  iterator finish;
  iterator end_of_storage;
};
