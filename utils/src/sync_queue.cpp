#include "utils/sync_queue.h"

void SyncQueue::push(const std::vector<uint8_t> &data_) {
  std::unique_lock<std::mutex> lock(this->mutex_);
  if (this->buffer_.size() + data_.size() > this->max_size_) {
    LOG(ERROR) << "SyncQueue push exceed max size";
    return;
  }
  this->buffer_.insert(this->buffer_.end(), data_.begin(), data_.end());
  this->cv_.notify_all();
}

auto SyncQueue::pop() -> std::vector<uint8_t> {
  std::unique_lock<std::mutex> lock(this->mutex_);
  if (this->buffer_.empty()) {
    this->cv_.wait(lock);
  }
  auto ret = this->buffer_;
  this->buffer_.clear();
  return ret;
}

void SyncQueue::finish() { this->cv_.notify_all(); }