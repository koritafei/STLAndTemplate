
#include <string>
template <class Key, class Compare = less<key>, class Alloc = alloc>
class set {
public:
  typedef Key key_type;
  typedef Key value_type;

  typedef Compare key_compare;
  typedef Compare value_compare;

private:
  typedef rb_tree<key_type,
                  value_type,
                  identify<value_type>,
                  key_compare,
                  Alloc>
      rep_type;

  rep_type t;

public:
  typedef typename rep_type::const_iterator iterator;
};

template <class Key, class T, class Compare = less<key>, class Alloc = alloc>
class map {
public:
  typedef Key                key_type;
  typedef T                  data_type;
  typedef T                  mapped_type;
  typedef pair<const Key, T> value_type;
  typedef Compare            key_compare;

private:
  typedef rb_tree<key_type,
                  value_type,
                  select1st<value_type>,
                  key_compare,
                  Alloc>
           req_type;
  req_type t;

public:
  typedef typename req_type::iterator iterator;
};

template <class Arg, class Result>
struct unary_function {
  typedef Arg    argument_type;
  typedef Result result_type;
};

template <class Arg1, class Arg2, class Result>
struct binary_function {
  typedef Arg1   first_argument_type;
  typedef Arg2   second_argument_type;
  typedef Result result_type;
};

template <class Operator, class T>
inline bind2nd<Operator>(const Operator &op, const T &x) {
  typedef typename Operator::second_argument_type arg2_type;
  return bind2nd<Operator>(op, arg2_type(x));
}
