#include "utils/ThreadPool.h"

ThreadPool::ThreadPool() {
  this->cleaner_ = std::make_unique<std::thread>([&]() {
    while (!this->is_close_ || !this->threads_.empty()) {
      std::unique_lock lock(this->mutex_);
      if (this->threads_.empty()) {
        this->cv_.wait(lock);
      }
      if (this->threads_.empty()) {
        continue;
      }
      this->threads_.front()->join();
      this->threads_.pop_front();
    }
  });
}

ThreadPool::~ThreadPool() {
  this->Close();
}

void ThreadPool::AddThread(std::function<void()> func) {
  std::unique_lock lock(this->mutex_);
  this->threads_.push_back(std::make_unique<std::thread>(func));
  this->cv_.notify_one();
}

void ThreadPool::Close() {
  this->is_close_ = true;
  this->cv_.notify_one();
  this->cleaner_->join();
}