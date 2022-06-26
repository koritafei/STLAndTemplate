`EBO`: `Empty Base Optimization`
## 空类
定一个空类，没有成员变量，没有继承，没有数据元素类。
```cpp
class Empty {
public:
  void print() {
    std::cout << "I am Empty class" << std::endl;
  }
};

int main(int argc, char **argv) {
  std::cout << sizeof(Empty) << std::endl; // 1
}
```
因为空类同样可以被实例化，每个实例在内存中都有一个独一无二的地址，为了达到这个目的，编译器往往会给一个空类隐含的加一个字节，这样空类在实例化后在内存得到了独一无二的地址．所以上述大小为1.

空基类地址是否相同：
```cpp
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

int main(int argc, char **argv) {
  Empty a, b;
  assert(!isSame(a, b));  // 编译通过，a与b的地址不同

  Empty *p = new Empty;
  Empty *q = new Empty;
  assert(!isSame(p, q));  // 编译通过，a与b的地址不同

  std::cout << sizeof(Empty) << std::endl;
}
```
两个不同对象地址是不同的.

### 空基类优化
现在对比一下下面两个用法，第一种，一个类中包含了两一个类作为成员，然后通过这个来获得被包含类的功能。
```cpp
class notEbo  {
    int i;
    Empty e;
    // do other things
};
```
另一种直接采用继承的方式来获得基类的成员函数及其他功能等等。
```cpp
class ebo:public Empty {
    int i;
    // do other things
};
```
第一种，会因为字节对齐，将其原来只占1字节，进行扩充到4的倍数，最后就是8字节。

对比这两个发现，第二种通过继承方式来获得基类的功能，并没有产生额外大小的优化称之为EBO(空基类优化)。
### STL中的EBO世界
```cpp
std::allocator<_Tp>
__gnu_cxx::bitmap_allocator<_Tp>
__gnu_cxx::bitmap_allocator<_Tp>
__gnu_cxx::__mt_alloc<_Tp>
__gnu_cxx::__pool_alloc<_Tp>
__gnu_cxx::malloc_allocator<_Tp>
```

### 利用EBO,手动实现一个简单的内存分配与释放
首先定义一个sizeof(class)=1的类，同STL一样，里面使用allocate与deallocate来进行内存管理。

```cpp
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
```
