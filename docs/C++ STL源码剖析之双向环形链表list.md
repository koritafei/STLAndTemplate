## `list`双向环形列表
![_list](./images/_list.png)
### `list`源码
类结构:
```cpp
template <typename _Tp, typenmae _Alloc=std::allocator<_Tp>>
class list : proteced _List_base<_Tp,_Alloc>{}
```
#### 构造函数
#### 不带任何元素的`list`
```cpp
explicit list(const allocator_type &__a) _GLIBCXX_NOEXCEPT: _Base(_Node_alloc_type(__a)) {
}
```
#### 带有`n`个元素且赋初值的list
```cpp
explicit list(size_type __n, const value_type &__value = value_type(),const allocator_type &__a = allocator_type()) : _Base(_Node_alloc_type(__a)) {
   _M_fill_initialize(__n, __value); 
}
```
#### 从一个范围初始化`lsit`
```cpp
template<typename _InputIterator>
list(_InputIterator __first, _InputIterator __last,
     const allocator_type &__a = allocator_type())
        : _Base(_Node_alloc_type(__a)) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename std::__is_integer<_InputIterator>::__type _Integral;
    _M_initialize_dispatch(__first, __last, _Integral());
}
```
#### 创建节点
创建一个新的节点并分配内存返回节点
```cpp
_Node *_M_create_node(const value_type &__x) {
  _Node *p = this->_M_get_node();
  __try{
    _M_get_Tp_allocator().construct(std::__addressof(__p->__M_data), __x);
  }__catch(...) {
    _M_put_node(__p);
    __throw_exception_again;
  }
  return __p;
}
```
`_M_get_node与_M_put_node`继承自基类，实现如下：
```cpp
_List_node<_Tp> *_M_get_node() {
  return _M_impl._Node_alloc_type::allocate(1);
}

void _M_put_node(_List_node<_Tp> *__p) _GLIBCXX_NOEXCEPT {
  _M_impl._Node_alloc_type::deallocate(__p,1);
}
```
#### 插入节点
* 在尾部插入`n`个值的节点,对应函数`_M_fill_initialize`
```cpp
explicit list(size_type __n, const value_type &__value = __value_type(), const allocator_type &__a = allocator_type()) : _Base(_Node_alloc_type(__a)) {
  _M_fill_initialize(__n, __value);
}
```
* 在指定位置插入节点，对应的函数为`_M_insert()`
`_M_fill_initialize`的实现如下：
```cpp
void _M_fill_initialize(size_type __n, const value_type &__x) {
  for(;__n;--__n) {
    push_back(__x);
  }
}
```
`push_back`实现如下:
```cpp
void push_back(const value_type &__x) {
  this->_M_insert(end(), __x);
}
```
`_M_insert`的实现：
```cpp
void _M_insert(iterator __position, const value_type &__x) {
  _Node *__tmp = _M_create_node(__x);
  __tmp->_M_hook(__position._M_node);
}
```
`_M_hook`实现如下：
```cpp
// 在指定的位置前插入this指向的节点
void_List_node_base::_M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT {
  this->_M_next = __position;		
  this->_M_prev = __position->_M_prev;
  __position->_M_prev->_M_next = this;
  __position->_M_prev = this;
}
```
在指定位置插入初始值为`__x`的节点:
```cpp
void _M_insert(iterator __position, const value_type &__x) {
    _Node *__tmp = _M_create_node(__x);
    __tmp->_M_next = __position;			// 第一步
    __tmp->_M_prev = __position->_M_prev;	// 第二步
    __position->_M_prev->_M_next = __tmp;	// 第三步
    __position->_M_prev = __tmp;			// 第四步
}
```
在末尾插入`n`个节点值为`x`的节点:
```cpp
void _M_fill_initialize(size_type __n, const value_type &__x) {
	for (; __n; --__n)
		_M_insert(end(), __x);
}
```
`insert`函数可以有以下三种实现形式:
* 指定迭代器前插入指定元素的值
```cpp
template<typename _Tp, typename _Alloc>
typename list<_Tp, _Alloc>::iterator
list<_Tp, _Alloc>::
#if __cplusplus >= 201103L
insert(const_iterator __position, const value_type& __x)
#else
insert(iterator __position, const value_type& __x)
#endif
{
  _Node* __tmp = _M_create_node(__x);
  __tmp->_M_hook(__position._M_const_cast()._M_node);
  return iterator(__tmp);
}
```
* 在指定迭代器之前插入n个指定节点值的节点。
```cpp
void insert(iterator __position, size_type __n, const value_type &__x) {
    list __tmp(__n, __x, get_allocator());
    splice(__position, __tmp);
}
```
实现是先调用list构造函数，完成创建一个拥有n个指定节点值的list。
```cpp
explicit list(size_type __n, const value_type &__value = value_type(),const allocator_type &__a = allocator_type()) : _Base(_Node_alloc_type(__a)) 
{ _M_fill_initialize(__n, __value); }
```
然后使用splice函数完成从另一个list中来插入当前list。
```cpp
void splice(iterator __position, list &__x) {
    if (!__x.empty()) {
        _M_check_equal_allocators(__x);

        this->_M_transfer(__position._M_const_cast(),
                          __x.begin(), __x.end());
    }
}
```
其中_M_transfer追踪代码，可以知道：
```cpp
// Moves the elements from [first,last) before position.
void
_M_transfer(iterator __position, iterator __first, iterator __last) {
    __position._M_node->_M_transfer(__first._M_node, __last._M_node);
}

void _List_node_base::_M_transfer(_List_node_base * const __first, _List_node_base * const __last) _GLIBCXX_USE_NOEXCEPT {
  if (this != __last) {
      // Remove [first, last) from its old position.
      __last->_M_prev->_M_next  = this;
      __first->_M_prev->_M_next = __last;
      this->_M_prev->_M_next    = __first;

      // Splice [first, last) into its new position.
      _List_node_base* const __tmp = this->_M_prev;
      this->_M_prev                = __last->_M_prev;
      __last->_M_prev              = __first->_M_prev;
      __first->_M_prev             = __tmp;
  }
}
```
* 从一个list范围把数据插入到指定迭代器前面
```cpp
template<typename _InputIterator>
void
insert(iterator __position, _InputIterator __first,
       _InputIterator __last) {
    list __tmp(__first, __last, get_allocator());
    splice(__position, __tmp);
}
```
#### 删除节点
1. 通过迭代器删除
基于`_M_erase`函数:
```cpp
this->_M_erase(begin()); 	// pop_front 不断删除起点的元素
this->_M_erase(iterator(this->_M_impl._M_node._M_prev)); // pop_back移除最后一个元素

_M_erase(iterator __position) {
  iterator __ret = iterator(__position._M_node->_M_next);
  _M_erase(__position._M_const_cast());
  return __ret;
}
```
2. 通过指定值删除
```cpp
template<typename _Tp, typename _Alloc>
void list<_Tp, _Alloc>::remove(const value_type& __value) {
  iterator __first = begin();
  iterator __last = end();
  iterator __extra = __last;
  while (__first != __last)
  {
      iterator __next = __first;
      ++__next;
      if (*__first == __value)
        {
          // _GLIBCXX_RESOLVE_LIB_DEFECTS
          // 526. Is it undefined if a function in the standard changes
          // in parameters?
          if (std::__addressof(*__first) != std::__addressof(__value))
        _M_erase(__first);
          else
        __extra = __first;
        }
      __first = __next;
  }
  if (__extra != __last)
	_M_erase(__extra);
}
```
`remove_if`函数:
```cpp
template<typename _Tp, typename _Alloc>
template <typename _Predicate>
void list<_Tp, _Alloc>::
remove_if(_Predicate __pred)
{
    iterator __first = begin();
    iterator __last = end();
	while (__first != __last)
    {
        iterator __next = __first;
        ++__next;
        if (__pred(*__first))
          _M_erase(__first);
        __first = __next;
    }
}
```
#### 删除一系列节点
1. 删除指定节点
```cpp
_M_erase(iterator __position)
_GLIBCXX_NOEXCEPT
{
    __position._M_node->_M_unhook();
    _Node *__n = static_cast<_Node *>(__position._M_node);
#if __cplusplus >= 201103L
    _M_get_Node_allocator().destroy(__n);
#else
    _M_get_Tp_allocator().destroy(std::__addressof(__n->_M_data));
#endif
    _M_put_node(__n);		// 释放内存
}
```
2. 删除系列节点
```cpp
iterator
#if __cplusplus >= 201103L
erase(const_iterator __first, const_iterator __last) noexcept
#else
erase(iterator __first, iterator __last)
#endif
{
    while (__first != __last)
        __first = erase(__first);
    return __last._M_const_cast();
}
```
3. 删除所有元素
```cpp
void clear()
_GLIBCXX_NOEXCEPT
{
    _Base::_M_clear();
    _Base::_M_init();
}

_List_base<_Tp, _Alloc>::
_M_clear() _GLIBCXX_NOEXCEPT
{
  typedef _List_node<_Tp>  _Node;
  _Node* __cur = static_cast<_Node*>(_M_impl._M_node._M_next);
  while (__cur != &_M_impl._M_node)
    {
      _Node* __tmp = __cur;			// 保存节点
      __cur = static_cast<_Node*>(__cur->_M_next);	// 往后遍历
    #if __cplusplus >= 201103L
      _M_get_Node_allocator().destroy(__tmp);
    #else
      _M_get_Tp_allocator().destroy(std::__addressof(__tmp->_M_data));
    #endif
      _M_put_node(__tmp);	// 释放内存
    }
}

void _M_init()
_GLIBCXX_NOEXCEPT
{
    this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
    this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
}
```
#### 元素访问
1. 访问第一个元素
```cpp
reference front()
_GLIBCXX_NOEXCEPT
{ return *begin(); }
const_reference
front() const
_GLIBCXX_NOEXCEPT
{ return *begin(); }
```
2. 访问最后一个元素
```cpp
reference
back()
_GLIBCXX_NOEXCEPT
{
    iterator __tmp = end();
    --__tmp;
    return *__tmp;
}
const_reference
back() const
_GLIBCXX_NOEXCEPT
{
    const_iterator __tmp = end();
    --__tmp;
    return *__tmp;
}
```
### 算法
1. `unique`
从容器中的每个连续的相等元素组中除去除第一个元素外的所有元素。
只有与列表容器紧邻的元素相比，该元素才从列表容器中删除
```cpp
template<typename _Tp, typename _Alloc>
template <typename _BinaryPredicate>
  void
  list<_Tp, _Alloc>::
  unique(_BinaryPredicate __binary_pred)
  {
    iterator __first = begin();
    iterator __last = end();
    if (__first == __last)
  return;
    iterator __next = __first;
    while (++__next != __last)
  {
    // 满足条件就删除
    if (__binary_pred(*__first, *__next))
       // 删除
      _M_erase(__next);
    else
      __first = __next;
    __next = __first;
  }
  }
```
2. `merge`
```cpp
template<typename _Tp, typename _Alloc>
void
list<_Tp, _Alloc>::
#if __cplusplus >= 201103L
merge(list&& __x)
#else
merge(list& __x)
#endif
{
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 300. list::merge() specification incomplete
  if (this != &__x)
  {
      _M_check_equal_allocators(__x);

      iterator __first1 = begin();
      iterator __last1 = end();
      iterator __first2 = __x.begin();
      iterator __last2 = __x.end();
      while (__first1 != __last1 && __first2 != __last2)
        if (*__first2 < *__first1)
         {
            iterator __next = __first2;
            _M_transfer(__first1, __first2, ++__next);
            __first2 = __next;
         }
        else
          ++__first1;
      if (__first2 != __last2)
        _M_transfer(__last1, __first2, __last2);
   }
}
```
3. `sort`
```cpp
void splice(const_iterator __position, list& __x, const_iterator __i) noexcept
{ splice(__position, std::move(__x), __i); }

void
splice(iterator __position, list &__x, iterator __i)
{
    iterator __j = __i._M_const_cast();
    ++__j;
    if (__position == __i || __position == __j)
        return;

    if (this != &__x)
        _M_check_equal_allocators(__x);

    this->(__position._M_const_cast(),
                      __i._M_const_cast(), __j);
}

void
_List_node_base::swap(_List_node_base& __x,
          _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT
{
      if ( __x._M_next != &__x )
    {
      if ( __y._M_next != &__y )
        {
          // Both __x and __y are not empty.
          std::swap(__x._M_next,__y._M_next);
          std::swap(__x._M_prev,__y._M_prev);
          __x._M_next->_M_prev = __x._M_prev->_M_next = &__x;
          __y._M_next->_M_prev = __y._M_prev->_M_next = &__y;
        }
      else
        {
              // __x is not empty, __y is empty.
              __y._M_next = __x._M_next;
              __y._M_prev = __x._M_prev;
              __y._M_next->_M_prev = __y._M_prev->_M_next = &__y;
              __x._M_next = __x._M_prev = &__x;
            }
        }
      else if ( __y._M_next != &__y )
        {
          // __x is empty, __y is not empty.
          __x._M_next = __y._M_next;
          __x._M_prev = __y._M_prev;
          __x._M_next->_M_prev = __x._M_prev->_M_next = &__x;
          __y._M_next = __y._M_prev = &__y;
        }
}


template<typename _Tp, typename _Alloc>
void
list<_Tp, _Alloc>::
sort() {
    // Do nothing if the list has length 0 or 1.
    if (this->_M_impl._M_node._M_next != &this->_M_impl._M_node
        && this->_M_impl._M_node._M_next->_M_next != &this->_M_impl._M_node) {
        list __carry; // 辅助链表，用于从a中提取元素以及临时保存两个链表的合并结果
        list __tmp[64]; // 保存着当前每一个归并层次的结果, i号链表保存的元素个数为2的i次方或者0
        list *__fill = &__tmp[0]; // 表示当前最大归并排序的层次，while循环之后__fill变成log2(list.size())
        list *__counter;

        do {
            __carry.splice(__carry.begin(), *this, begin()); //把当前链表的第一个节点放在carry链表头

            for (__counter = &__tmp[0];
                 __counter != __fill && !__counter->empty();
                 ++__counter) {
                __counter->merge(__carry);	// 两个有序链表合并
                __carry.swap(*__counter); // 类似于交换链表carry和counter[i]内容	
            }
            __carry.swap(*__counter); // 类似于交换链表carry和counter[i]内容	
            if (__counter == __fill)
                ++__fill;
        } while (!empty());
		// 每两个进行归并，依次网上，直到最后*(__fill-1)保存最后的排序结果。然后交换到当前list中。
        for (__counter = &__tmp[1]; __counter != __fill; ++__counter)
            __counter->merge(*(__counter - 1));
        swap(*(__fill - 1)); 
    }
}
```
## `list's Iterator`剖析
定义
```cpp
template<typename _Tp>
struct _List_iterator
{
  typedef _List_iterator<_Tp>                _Self;
  typedef _List_node<_Tp>                    _Node;

  typedef ptrdiff_t                          difference_type;
  typedef std::bidirectional_iterator_tag    iterator_category;
  typedef _Tp                                value_type;
  typedef _Tp*                               pointer;
  typedef _Tp&                               reference;

   // The only member points to the %list element.
   __detail::_List_node_base* _M_node;         
   //  _List_node(节点的数据部分) -> _List_node_base(前指针与后指针)

  _List_iterator() _GLIBCXX_NOEXCEPT
  : _M_node() { }

  explicit
  _List_iterator(__detail::_List_node_base* __x) _GLIBCXX_NOEXCEPT
  : _M_node(__x) { }

  _Self
  _M_const_cast() const _GLIBCXX_NOEXCEPT
  { return *this; }
 
  // The only member points to the %list element.
  __detail::_List_node_base* _M_node;
};
```
内部重载
```cpp
// Must downcast from _List_node_base to _List_node to get to _M_data.
// 重载*操作符
reference operator*() const _GLIBCXX_NOEXCEPT
{ 
	return static_cast<_Node*>(_M_node)->_M_data; 
}

// 重载->操作符
pointer operator->() const _GLIBCXX_NOEXCEPT
{ 
	return std::__addressof(static_cast<_Node*>(_M_node)->_M_data); 
}

// 重载前置++操作符  ++i
_Self& operator++() _GLIBCXX_NOEXCEPT   
{ 
	_M_node = _M_node->_M_next;
	return *this;
}

// 重载后置++操作符 i++
_Self operator++(int) _GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;			 // 记录原值  *调用的是拷贝构造函数
    _M_node = _M_node->_M_next;		 // 进行操作
    return __tmp;					 // 返回原值
}

// 重载前置--操作符  --i
_Self& operator--() _GLIBCXX_NOEXCEPT
{
	_M_node = _M_node->_M_prev;
	return *this;
}
// 重载后置--操作符  --i
_Self operator--(int) _GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;
    _M_node = _M_node->_M_prev;
    return __tmp;
}
// 重载++操作符
bool operator==(const _Self& __x) const _GLIBCXX_NOEXCEPT
{ 
	return _M_node == __x._M_node; 
}
// 重载!=操作符
bool operator!=(const _Self& __x) const _GLIBCXX_NOEXCEPT
{ 
	return _M_node != __x._M_node; 
}
```
## 节点设计
```cpp
/// An actual node in the %list.
template<typename _Tp>
struct _List_node : public __detail::_List_node_base
{
  ///< User's data.
  _Tp _M_data;

#if __cplusplus >= 201103L
  template<typename... _Args>
    _List_node(_Args&&... __args)
: __detail::_List_node_base(), _M_data(std::forward<_Args>(__args)...) 
    { }
#endif
};

namespace __detail
{
    _GLIBCXX_BEGIN_NAMESPACE_VERSION
    /// Common part of a node in the %list. 
    struct _List_node_base
    {
      _List_node_base* _M_next;
      _List_node_base* _M_prev;

      static void
      swap(_List_node_base& __x, _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_transfer(_List_node_base* const __first,
          _List_node_base* const __last) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_reverse() _GLIBCXX_USE_NOEXCEPT;

      void
      _M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_unhook() _GLIBCXX_USE_NOEXCEPT;
    };

    _GLIBCXX_END_NAMESPACE_VERSION
} // namespace detail
```

