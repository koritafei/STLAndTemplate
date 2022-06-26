#include <iostream>

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidiectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidiectional_iterator_tag {};

// 继承的好处就是，当函数需要用 input_iterator_tag 的时候
// 假设你传进一个forward_iterator_tag，它会沿继承向上找，知道符合条件

// traits 型别
template <class T>
struct Iterator_traits {
  typedef typename T::iterator_category iterator_category;
};

// 针对原生指针的偏特化
template <class I>
struct Iterator_traits<I *> {
  typedef random_access_iterator_tag iterator_category;
};

template <typename I>
struct Iterator_traits<const I *> {
  typedef random_access_iterator_tag iterator_category;
};

template <typename inputIterator, class distance>
inline typename Iterator_traits<inputIterator>::iterator_category
__advance(inputIterator &i, distance n, input_iterator_tag) {
  std::cout << "Input tag" << std::endl;
  return input_iterator_tag();
}

template <typename outputIterator, class distance>
inline typename Iterator_traits<outputIterator>::iterator_category
__advance(outputIterator &i, distance n, output_iterator_tag) {
  std::cout << "outputIterator tag" << std::endl;
  return output_iterator_tag();
}

template <typename forwardIterator, typename distance>
inline typename Iterator_traits<forwardIterator>::iterator_category
__advance(forwardIterator &i, distance n, forward_iterator_tag) {
  std::cout << "forward_iterator_tag" << std::endl;
  return forward_iterator_tag();
}

template <typename BidiectionalIterator, typename distance>
inline typename Iterator_traits<BidiectionalIterator>::iterator_category
__advance(BidiectionalIterator &i, distance n, bidiectional_iterator_tag) {
  std::cout << "bidirectional_iterator_tag" << std::endl;
  return bidiectional_iterator_tag();
}

template <typename RandomAccessIterator, typename distance>
inline typename Iterator_traits<RandomAccessIterator>::iterator_category
__advance(RandomAccessIterator &i, distance n, random_access_iterator_tag) {
  std::cout << "random_access_iterator_tag" << std::endl;
  return random_access_iterator_tag();
}

// 对外接口
template <typename InputIterator, typename Distance>
inline typename Iterator_traits<InputIterator>::iterator_category advance(
    InputIterator &i,
    Distance       n) {
  // 通过Ierator_traits询问它的iterator_category是谁
  typedef typename Iterator_traits<InputIterator>::iterator_category category;
  return __advance(i, n, category());  // 各类型的重载
}

// 定义class type
template <typename Category>
struct iterator {
  typedef Category iterator_category;
};

int main() {
  iterator<input_iterator_tag>         input;
  iterator<output_iterator_tag>        output;
  iterator<forward_iterator_tag>       forward;
  iterator<bidiectional_iterator_tag>  bidect;
  iterator<random_access_iterator_tag> random;
  input_iterator_tag                   inputIteratorTag  = advance(input, 10);
  output_iterator_tag                  outputIteratorTag = advance(output, 10);
  forward_iterator_tag       forwardIteratorTag          = advance(forward, 10);
  bidiectional_iterator_tag  bidiectionalIteratorTag     = advance(bidect, 10);
  random_access_iterator_tag randomAccessIteratorTag     = advance(random, 10);
  int                       *p                           = NULL;
  random_access_iterator_tag v                           = advance(p, 10);
  return 0;
}