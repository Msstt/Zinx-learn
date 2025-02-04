#include "gtest/gtest.h"
#include "utils/GlobalObject.h"

TEST(config, basic) {
  GlobalObject::Instance().LoadTestConfig("test");

  ASSERT_EQ(GlobalObject::Instance().name_, "TEST");
  ASSERT_EQ(GlobalObject::Instance().host_, "127.0.0.1");
  ASSERT_EQ(GlobalObject::Instance().port_, 7777);
  ASSERT_EQ(GlobalObject::Instance().max_connection_number_, 5);
}