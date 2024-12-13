#pragma once
#include <boost/asio.hpp>
#include <glog/logging.h>
#include <iostream>

using namespace boost::asio;
using namespace boost::system;

#define CREATE_THREAD auto future = std::async(std::launch::async, [&]()
#define CREATE_THREAD__(x) auto future = std::async(std::launch::async, [&, x]()
#define CREATE_THREAD_ );

void LittleEndianWrite(const uint32_t &, uint8_t *);
void LittleEndianRead(const uint8_t *, uint32_t &);

auto SocketExactRead(ip::tcp::socket &, std::vector<uint8_t> &) -> bool;
auto SocketExactWrite(const std::vector<uint8_t> &, ip::tcp::socket &) -> bool;