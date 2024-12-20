#pragma once
#include <thread>
#include <vector>
#include <mutex>

class ThreadPool {
public:
  static auto Instance() -> ThreadPool &;
  void AddThread(std::thread thread);

private:
  ThreadPool() = default;

  std::mutex mutex_{};
  std::vector<std::thread> thread_{};
};