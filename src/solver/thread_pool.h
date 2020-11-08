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
  std::queue<std::function<void()>> tasks_;

  // synchronization
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;
};

// the constructor just launches some amount of workers_
inline ThreadPool::ThreadPool(const size_t thread_count) : stop_(false) {
  for (size_t i = 0; i < thread_count; ++i)
    workers_.emplace_back([this] {
      for (;;) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(this->queue_mutex_);
          this->condition_.wait(lock, [this] { return this->stop_ || !this->tasks_.empty(); });
          if (this->stop_ && this->tasks_.empty())
            return;
          task = std::move(this->tasks_.front());
          this->tasks_.pop();
        }

        task();
      }
    });
}

// add new work item to the pool
template <class F, class... Args>
std::future<typename std::invoke_result_t<F, Args...>> ThreadPool::push(F&& f, Args&&... args) {
  using return_type = typename std::invoke_result_t<F, Args...>;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    // don't allow pushing after stopping the pool
    if (stop_)
      throw std::runtime_error("push on stopped ThreadPool");

    tasks_.emplace([task]() { (*task)(); });
  }
  condition_.notify_one();
  return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_.notify_all();
  for (std::thread& worker : workers_)
    worker.join();
}

#endif   // !THREAD_POOL_H_
