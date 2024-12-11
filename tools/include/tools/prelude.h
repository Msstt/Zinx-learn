#pragma once
#include <glog/logging.h>

#define CREATE_THREAD auto future = std::async(std::launch::async, [&]()
#define CREATE_THREAD__(x) auto future = std::async(std::launch::async, [&, x]()
#define CREATE_THREAD_ );