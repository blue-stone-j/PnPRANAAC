// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

// This implementation is based on the implementation found here:
// https://github.com/progschj/ThreadPool


#ifndef THEIA_UTIL_THREADPOOL_H_
#define THEIA_UTIL_THREADPOOL_H_

#include <glog/logging.h>

#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "theia/util/util.h"

namespace theia {

// A simple threadpool implementation.
class ThreadPool {
 public:
  // All the threads are created upon construction.
  explicit ThreadPool(const int num_threads);
  ~ThreadPool();

  // Adds a task to the threadpool.
  template <class F, class... Args>
  auto Add(F&& f, Args&& ... args)
      ->std::future<typename std::result_of<F(Args...)>::type>;

 private:
  // Keep track of threads so we can join them
  std::vector<std::thread> workers;
  // The task queue
  std::queue<std::function<void()> > tasks;

  // Synchronization
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;

  DISALLOW_COPY_AND_ASSIGN(ThreadPool);
};

// add new work item to the pool
template <class F, class... Args>
auto ThreadPool::Add(F&& f, Args&& ... args)
    ->std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()> >(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex);

    // don't allow enqueueing after stopping the pool
    CHECK(!stop) << "The ThreadPool object has been destroyed! Cannot add more "
                    "tasks to the ThreadPool!";

    tasks.emplace([task]() {
      (*task)();
    });
  }
  condition.notify_one();
  return res;
}

}  // namespace theia

#endif  // THEIA_UTIL_THREADPOOL_H_
