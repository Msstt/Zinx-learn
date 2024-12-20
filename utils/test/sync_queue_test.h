#include "utils/sync_queue.h"
#include "gtest/gtest.h"

TEST(sync_queue, basic) {
  SyncQueue queue(10);
  std::thread thread1([&]() {
    for (size_t i = 0; i < 10; i++) {
      queue.push({static_cast<uint8_t>(i)});
    }
  });
  std::thread thread2([&]() {
    std::vector<uint8_t> buffer;
    while (buffer.size() < 10) {
      auto data = queue.pop();
      buffer.insert(buffer.end(), data.begin(), data.end());
    }
    for (size_t i = 0; i < 9; i++) {
      ASSERT_EQ(buffer[i], i);
    }
  });
  thread1.join();
  thread2.join();
}