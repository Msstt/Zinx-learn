#include "utils/ThreadPool.h"

auto ThreadPool::Instance() -> ThreadPool & {
  static ThreadPool instance;
  return instance;
}

void ThreadPool::AddThread(std::thread thread) {
  std::unique_lock<std::mutex> lock(this->mutex_);
  this->thread_.emplace_back(std::move(thread));
}