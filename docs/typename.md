## `typename`
在`STL`的底层有如下定义，`typedef和typename`混用
```cpp
template<typename _Iterator>
struct iterator_traits
{
  typedef typename _Iterator::iterator_category iterator_category;
  typedef typename _Iterator::value_type        value_type;
  typedef typename _Iterator::difference_type   difference_type;
  typedef typename _Iterator::pointer           pointer;
  typedef typename _Iterator::reference         reference;
};
```
对于用于模板定义的依赖于模板参数的名称，只有在实例化的参数中存在这个类型名，或者这个名称前使用了typename关键字来修饰，编译器才会将该名称当成是类型。除了以上这两种情况，绝不会被当成是类型。

