#include "utils/GlobalObject.h"
#include "gtest/gtest.h"

TEST(config, basic) {
  ASSERT_EQ(GlobalObject::Instance().name_, "ZINX");
  ASSERT_EQ(GlobalObject::Instance().host_, "127.0.0.1");
  ASSERT_EQ(GlobalObject::Instance().port_, 7777);
  ASSERT_EQ(GlobalObject::Instance().max_connection_number_, 5);
}