#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <iterator>
#include <map>
#include <queue>

#include <chrono>
#include <assert.h>
#include <new>
#include <functional>

#include <atomic>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <future>
#include <unistd.h>
#include <numeric>
#include <unistd.h>
#include <algorithm>
#include <climits>
#include <future>
#include <math.h>
#include <exception>
#include <atomic>
// #include <experimental/future>

#include "myutils.hh"

using namespace std;


// 异常安全的代码
class join_threads {
private:
    std::vector<std::thread>& threads;
public:
    explicit join_threads(std::vector<std::thread>& threads_) : threads(threads_) {}
    ~join_threads() {
        for (unsigned long i = 0; i < threads.size(); ++i) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
    }
};

struct barrier
{
  std::atomic<unsigned> count;
  std::atomic<unsigned> spaces;
  std::atomic<unsigned> generation;
  barrier(unsigned count_):
    count(count_),spaces(count_),generation(0)
  {}
  void wait()
  {
    unsigned const gen=generation.load();
    if(!--spaces)
    {
      spaces=count.load();
      ++generation;
    }
    else
    {
      while(generation.load()==gen)
      {
        std::this_thread::yield();
      }
    }
  }
  void done_waiting()
  {
    --count;
    if(!--spaces)
    {
      spaces=count.load();
      ++generation;
    }
  }
};
template<typename Iterator>
void parallel_partial_sum(Iterator first,Iterator last)
{
  typedef typename Iterator::value_type value_type;
  struct process_element  // 1
  {
    void operator()(Iterator first,Iterator last,
                    std::vector<value_type>& buffer,
                    unsigned i,barrier& b)
    {
      value_type& ith_element=*(first+i);
      bool update_source=false;
      for(unsigned step=0,stride=1;stride<=i;++step,stride*=2)
      {
        value_type const& source=(step%2)?  // 2
          buffer[i]:ith_element;
        value_type& dest=(step%2)?
          ith_element:buffer[i];
        value_type const& addend=(step%2)?  // 3
          buffer[i-stride]:*(first+i-stride);
        dest=source+addend;  // 4
        update_source=!(step%2);
        b.wait();  // 5
      }
      if(update_source)  // 6
      {
        ith_element=buffer[i];
      }
      b.done_waiting();  // 7
    }
  };
  unsigned long const length=std::distance(first,last);
  if(length<=1)
    return;
  std::vector<value_type> buffer(length);
  barrier b(length);
  std::vector<std::thread> threads(length-1);  // 8
  join_threads joiner(threads);
  Iterator block_start=first;
  for(unsigned long i=0;i<(length-1);++i)
  {
    threads[i]=std::thread(process_element(),first,last,  // 9
                           std::ref(buffer),i,std::ref(b));
  }
  process_element()(first,last,buffer,length-1,b);  // 10
}


void test1() {
    std::vector<long> input_int;
    for (unsigned i = 1; i < 10; ++i) {
        input_int.push_back(i);
    }
    auto first = input_int.begin();
    auto last = input_int.end();
    unsigned long const length = std::distance(first, last); // 包括首尾元素
    cout << "first=" << *first << " last=" << *(last-1) << " end=" << *last<< " length=" << length << endl;

    std::vector<long> tmp(input_int);
    // utils_print_copy_decorate(tmp);
    std::vector<long> target(tmp.size());
    utils_consume_time_decorate(std::partial_sum(tmp.begin(), tmp.end(), target.begin());)
    // utils_print_copy_decorate(target);
    utils_consume_time_decorate(parallel_partial_sum(tmp.begin(), tmp.end());)
    // utils_print_copy_decorate(tmp);
    if (tmp != target) {
        cout << "parallel_partial_sum error." << endl;
    } else {
        cout << "parallel_partial_sum ok." << endl;
    }
    for (unsigned i = 0; i < tmp.size(); ++i) {
        if (tmp[i] != target[i]) {
            cout << "(" << input_int[i] << "," << target[i] << "," << tmp[i] << ")!!!" << endl;
        } else {
            cout << "(" << input_int[i] << "," << target[i] << "," << tmp[i] << ")" << endl;
        }
    }
}

int main() {
  test1();
  return 0;
}