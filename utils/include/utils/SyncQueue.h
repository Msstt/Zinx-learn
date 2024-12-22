#pragma once
#include <queue>

#include "utils/Prelude.h"

template <class T>
class SyncQueue {
 public:
  SyncQueue() = default;
  SyncQueue(uint32_t max_size) : max_size_(max_size) {}

  auto Empty() -> bool {
    std::lock_guard<std::mutex> lock(this->mutex_);
    return this->empty_();
  }

  auto Size() -> uint32_t {
    std::lock_guard<std::mutex> lock(this->mutex_);
    return this->queue_.size();
  }

  void Push(T item) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->full_cond_.wait(lock, [&]() { return !this->Full_(); });
    this->queue_.push(item);
    empty_cond_.notify_one();
  }

  auto Pop() -> T {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->empty_cond_.wait(lock, [&]() { return !this->Empty_(); });
    auto item = this->queue_.front();
    this->queue_.pop();
    full_cond_.notify_one();
    return item;
  }

  void Push(std::vector<T> items) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->full_cond_.wait(lock, [&]() {
      return this->queue_.size() + items.size() <= this->max_size_;
    });
    for (const auto& item : items) {
      this->queue_.push(item);
    }
    empty_cond_.notify_all();
  }

  auto PopAll() -> std::vector<T> {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->empty_cond_.wait(lock,
                           [&]() { return !this->Empty_() || this->finish_; });
    std::vector<T> ret;
    while (!this->queue_.empty()) {
      ret.push_back(this->queue_.front());
      this->queue_.pop();
    }
    full_cond_.notify_all();
    return ret;
  }

  void Finish() {
    this->finish_ = true;
    full_cond_.notify_all();
    empty_cond_.notify_all();
  }

 private:
  auto Empty_() -> bool { return this->queue_.size() == 0; }
  auto Full_() -> bool { return this->queue_.size() == max_size_; }

  bool finish_ = false;
  uint32_t max_size_;
  std::mutex mutex_{};
  std::condition_variable empty_cond_{};
  std::condition_variable full_cond_{};
  std::queue<T> queue_{};
};