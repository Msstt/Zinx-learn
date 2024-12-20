#pragma once
#include "utils/prelude.h"

class SyncQueue {
public:
  SyncQueue(uint32_t max_size) : max_size_(max_size) {}
  void push(const std::vector<uint8_t> &);
  auto pop() -> std::vector<uint8_t>;
  void finish();

private:
  uint32_t max_size_;
  std::vector<uint8_t> buffer_{};
  std::mutex mutex_{};
  std::condition_variable cv_{};
};