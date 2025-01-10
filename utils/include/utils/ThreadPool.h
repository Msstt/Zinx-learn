#pragma once
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <deque>
#include <functional>

class ThreadPool {
 public:
  ThreadPool();
  ~ThreadPool();
  void AddThread(std::function<void()>);
  void Close();

 private:
  std::atomic<bool> is_close_{false};
  std::mutex mutex_{};
  std::condition_variable cv_{};
  std::deque<std::unique_ptr<std::thread>> threads_{};
  std::unique_ptr<std::thread> cleaner_{};
};