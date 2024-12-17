#include "utils/prelude.h"
#include "gtest/gtest.h"

TEST(little_endian, basic) {
  std::vector<uint8_t> buf(8);
  LittleEndianWrite(123, buf.data());
  LittleEndianWrite(456, buf.data() + 4);
  uint32_t num;
  LittleEndianRead(buf.data(), num);
  ASSERT_EQ(num, 123);
  LittleEndianRead(buf.data() + 4, num);
  ASSERT_EQ(num, 456);
}