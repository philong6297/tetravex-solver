#ifndef SOLVER_THREAD_POOL_H_
#define SOLVER_THREAD_POOL_H_

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(size_t thread_count);

  template <class F, class... Args>
  std::future<typename std::invoke_result_t<F, Args...>> push(F&& f, Args&&... args);
  ~ThreadPool();

 private:
  // need to keep track of threads so we can join them
  std::vector<std::thread> workers_;
  // the task queue
  std::queue<std::function<void()>> tasks_queue_;

  // synchronization
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;
};

// the constructor just launches some amount of workers_
inline ThreadPool::ThreadPool(const size_t thread_count) : stop_(false) {
  // Create |thread_count| workers prepare for incomming tasks
  for (size_t i = 0; i < thread_count; ++i)
    // emplace back: in general, more efficient than push back
    workers_.emplace_back([this] {
      // infinity loop
      for (;;) {
        std::function<void()> incoming_task;

        {
          std::unique_lock<std::mutex> lock(this->queue_mutex_);
          // create condition for mutex when need to lock, unlock
          this->condition_.wait(lock,
                                [this] { return this->stop_ || !this->tasks_queue_.empty(); });

          // thread pool need to stop and there is no task left, the work need will do nothing
          if (this->stop_ && this->tasks_queue_.empty())
            return;

          // if thread pool want to stop but still have tasks, or the thread pool still running, the
          // worker will assign for new task
          incoming_task = std::move(this->tasks_queue_.front());

          // remove the task from queue
          this->tasks_queue_.pop();
        }

        // worker do the task
        incoming_task();
      }
    });
}

// add new work item to the pool, use the template to make the thread push can be use for any
// function: ThreadPool pool(5); int f(); void g(int a, int b); => k() : g = bind(k, a, b); class a
// { double b();}; ~ double b(A* a); pool.push(&f); pool.push(&g, a, b); push.push(&a::b, &a);
// variadic arguments
template <class FunctionName, class... FunctionArgs>
std::future<typename std::invoke_result_t<FunctionName, FunctionArgs...>> ThreadPool::push(
  FunctionName&& f,
  FunctionArgs&&... args) {

  using return_type = typename std::invoke_result_t<FunctionName, FunctionArgs...>;

  // create a packaged task async
  auto task = std::make_shared<std::packaged_task<return_type()>>(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();

  // assign task to queue
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    // don't allow pushing after stopping the pool
    if (stop_)
      throw std::runtime_error("push on stopped ThreadPool");

    tasks_queue_.emplace([task]() { (*task)(); });
  }

  // notify one waiting thread ~ 1 worker is relaxing to assign the task
  condition_.notify_one();

  return res;
}

// { // scope lifetime
//  int a;
//  double b;
// ThreadPool pool;
// pool.~ThreadPool();
// }
// the destructor joins all threads

// include "....H" ~ copy content of .h header
// coi .H interface, dinh nghia cac function , cpp -> source file

inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }

  // notify all the workers to stop and join
  condition_.notify_all();
  for (std::thread& worker : workers_)
    worker.join();
}

#endif   // !THREAD_POOL_H_
