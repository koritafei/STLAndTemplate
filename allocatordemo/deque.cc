
#include <cstddef>

template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator {
  typedef T      value_type;
  typedef Ptr    pointer;
  typedef Ref    reference;
  typedef size_t size_type;
  typedef T**    map_pointer;

  typedef __deque_iterator self;

  T*          cur;
  T*          first;
  T*          last;
  map_pointer node;
};

template <class T, class Alloc = alloc, size_t BufSiz = 0>
class deque {
public:
  typedef T                                   value_type;
  typedef value_type*                         pointer;
  typedef value_type&                         reference;
  typedef __deque_iterator<T, T&, T*, BufSiz> iterator;

  typedef size_t size_type;

protected:
  typedef pointer* map_pointer;

  iterator    start;
  iterator    finish;
  map_pointer map;
  size_type   map_size;
};
