#pragma once
#include <glog/logging.h>

#define CREATE_THREAD auto future = std::async(std::launch::async, [&]()
#define CREATE_THREAD_ );