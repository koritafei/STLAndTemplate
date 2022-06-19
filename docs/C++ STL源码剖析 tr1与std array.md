`C++ tr1`全称为`Technical Report 1`.
## `std::tr1::array`
使用：
```cpp
#include <tr1/array>
std::tr1::array<int, 10> a;
```
没有析构和构造函数。
静态数组的源码实现：
```cpp
template <typename _Tp, typename _Nm>
struct array {
  typedef _Tp                                   value_type;
  typedef value_type&                           reference;
  typedef const value_type&                     const_reference;
  typedef value_type*                           iterator;
  typedef const value_type*                     const_iterator;
  typedef std::size_t                           size_type;
  typedef std::ptrdiff_t                        difference_type;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
};
```
`tr1::array`支持传递数组大小为`0`;
```cpp
std::tr1::array<int, 0> a; // 合法，
```
内部做如下转换：
```cpp
value_type _M_instance[_Nm ? _Nm : 1];
```
对比`tr1`与`std`中的`array`:
```cpp
template <typename _Tp, typename _Nm>
struct array {
  typedef _Tp                                   value_type;
  typedef value_type*                           pointer;
  typedef const value_type*                     const_pointer;
  typedef value_type&                           reference;
  typedef const value_type&                     const_reference;
  typedef value_type*                           iterator;
  typedef const value_type*                     const_iterator;
  typedef std::size_t                           size_type;
  typedef std::ptrdiff_t                        difference_type;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // Support for zero-sized arrays mandatory.
  typedef _GLIBCXX_STD_C::__array_traits<_Tp, _Nm> _AT_Type;
  typename _AT_Type::_Type                         _M_elems;
};
```
在源码中对`__array_traits`的定义如下：
```cpp
template <typename _Tp, typename _Nm>
struct __array_traits{
  typedef _Tp _Type[_Nm];

  // 返回了数组的指针
  static constexpr _Tp& _S_ref(const _Type &__t, std::size_t __n) noexcept {
    return const_cast<_Tp&>(__t[__n]);
  }
};
```
对传进来的`size`使用模板进行处理，并使用偏特化处理了`size == 0` 的情况。
```cpp
// std对size==0的偏特化处理

template <typename _Tp>
struct __array_traits<_Tp, 0> {
  struct _Type{};

  static constexpr _Tp& _S_ref(const _Type &, std::size_t) { 
    return *static_case<_Tp*>(nullptr);
  }
};
```
