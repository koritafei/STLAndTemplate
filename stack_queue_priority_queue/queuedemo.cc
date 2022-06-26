#include <ctime>
#include <deque>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

void test_queue() {
  std::cout << "============test_queue=============" << std::endl;
  clock_t                         timeStart = clock();
  std::queue<int, std::list<int>> c;
  for (long i = 0; i < 100000; i++) c.push(i + 1);
  std::cout << "queue.size()= " << c.size() << std::endl;
  std::cout << "queue.front()= " << c.front() << std::endl;
  c.pop();
  std::cout << "queue.size()= " << c.size() << std::endl;
  std::cout << "queue.front()= " << c.front() << std::endl;
  std::cout << "use queue milli-seconds : " << (clock() - timeStart)
            << std::endl;

  timeStart = clock();
  std::queue<int, std::deque<int>> c1;
  for (long i = 0; i < 100000; i++) c1.push(i + 1);
  std::cout << "queue.size()= " << c1.size() << std::endl;
  std::cout << "queue.front()= " << c1.front() << std::endl;
  c1.pop();
  std::cout << "queue.size()= " << c1.size() << std::endl;
  std::cout << "queue.front()= " << c1.front() << std::endl;
  std::cout << "use queue milli-seconds : " << (clock() - timeStart)
            << std::endl;
}

int main(int argc, char **argv) {
  test_queue();
}