#include "config_test.h"
#include "little_endian_test.h"
#include "sync_queue_test.h"
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}