#include <ctime>
#include <deque>
#include <iostream>
#include <list>
#include <stack>
#include <vector>

void test_stack() {
  std::cout << "============test_stack=============" << std::endl;
  clock_t                         timeStart = clock();
  std::stack<int, std::list<int>> c;
  for (long i = 0; i < 100000; i++) c.push(i + 1);
  std::cout << "stack.size()= " << c.size() << std::endl;
  std::cout << "stack.top()= " << c.top() << std::endl;
  c.pop();
  std::cout << "stack.size()= " << c.size() << std::endl;
  std::cout << "stack.top()= " << c.top() << std::endl;
  std::cout << "use stack milli-seconds : " << (clock() - timeStart)
            << std::endl;

  timeStart = clock();
  std::stack<int, std::deque<int>> c1;
  for (long i = 0; i < 100000; i++) c1.push(i + 1);
  std::cout << "stack.size()= " << c1.size() << std::endl;
  std::cout << "stack.top()= " << c1.top() << std::endl;
  c1.pop();
  std::cout << "stack.size()= " << c1.size() << std::endl;
  std::cout << "stack.top()= " << c1.top() << std::endl;
  std::cout << "use stack milli-seconds : " << (clock() - timeStart)
            << std::endl;

  // vector可以作为stack的底层容器
  std::stack<int, std::vector<int>> c2;
  for (long i = 0; i < 100000; i++) c2.push(i + 1);
  std::cout << "stack.size()= " << c2.size() << std::endl;
  std::cout << "stack.top()= " << c2.top() << std::endl;
  c2.pop();
  std::cout << "stack.size()= " << c2.size() << std::endl;
  std::cout << "stack.top()= " << c2.top() << std::endl;
  std::cout << "use stack milli-seconds : " << (clock() - timeStart)
            << std::endl;
}

int main(int argc, char **argv) {
  test_stack();
}