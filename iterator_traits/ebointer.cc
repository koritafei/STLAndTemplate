#include <iostream>

class MyAllocator {
public:
  void *allocate(std::size_t size) {
    return std::malloc(size);
  }

  void deallocate(void *ptr) {
    std::free(ptr);
  }
};

template <class T, class Allocator>
class MyContainerNotEBO {
  T          *data_ = nullptr;
  std::size_t capacity_;
  Allocator   allocator_;  // 嵌入一个MyAllocator
public:
  MyContainerNotEBO(std::size_t capacity)
      : capacity_(capacity), allocator_(), data_(nullptr) {
    std::cout << "alloc malloc" << std::endl;
    data_ = reinterpret_cast<T *>(
        allocator_.allocate(capacity * sizeof(T)));  // 分配内存
  }

  ~MyContainerNotEBO() {
    std::cout << "MyContainerNotEBO free malloc" << std::endl;
    allocator_.deallocate(data_);
  }
};

template <class T, class Allocator>
class MyContainerEBO : public Allocator {  // 继承一个EBO
  T          *data_ = nullptr;
  std::size_t capacity_;

public:
  MyContainerEBO(std::size_t capacity) : capacity_(capacity), data_(nullptr) {
    std::cout << "alloc malloc" << std::endl;
    data_ = reinterpret_cast<T *>(this->allocate(capacity * sizeof(T)));
  }

  ~MyContainerEBO() {
    std::cout << "MyContainerEBO free malloc" << std::endl;
    this->deallocate(data_);
  }
};

int main() {
  MyContainerNotEBO<int, MyAllocator> notEbo =
      MyContainerNotEBO<int, MyAllocator>(0);
  std::cout << "Using Not EBO Test sizeof is " << sizeof(notEbo) << std::endl;
  MyContainerEBO<int, MyAllocator> ebo = MyContainerEBO<int, MyAllocator>(0);
  std::cout << "Using EBO Test sizeof is " << sizeof(ebo) << std::endl;

  return 0;
}