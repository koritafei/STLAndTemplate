#include <ctime>
#include <deque>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

void test_priority_queue() {
  std::cout << "============test_priority_priority_queue_demo============="
            << std::endl;
  clock_t                                    timeStart = clock();
  std::priority_queue<int, std::vector<int>> c;
  for (long i = 0; i < 100000; i++) c.push(i + 1);
  std::cout << "priority_queue.size()= " << c.size() << std::endl;
  std::cout << "priority_queue.top()= " << c.top() << std::endl;
  c.pop();
  std::cout << "priority_queue.size()= " << c.size() << std::endl;
  std::cout << "priority_queue.top()= " << c.top() << std::endl;
  std::cout << "use priority_queue milli-seconds : " << (clock() - timeStart)
            << std::endl;

  timeStart = clock();
  std::priority_queue<int, std::deque<int>> c1;
  for (long i = 0; i < 100000; i++) c1.push(i + 1);
  std::cout << "priority_queue.size()= " << c1.size() << std::endl;
  std::cout << "priority_queue.top()= " << c1.top() << std::endl;
  c1.pop();
  std::cout << "priority_queue.size()= " << c1.size() << std::endl;
  std::cout << "priority_queue.top()= " << c1.top() << std::endl;
  std::cout << "use priority_queue milli-seconds : " << (clock() - timeStart)
            << std::endl;
}

int main(int argc, char **argv) {
  test_priority_queue();
}