#pragma once
#include <glog/logging.h>
#include <boost/asio.hpp>
#include <iostream>

#include "utils/ThreadPool.h"
#include "utils/GlobalObject.h"
#include "utils/SyncQueue.h"

using namespace boost::asio;
using namespace boost::system;

enum class ErrorKind { OK_, EOF_, ERROR_ };

void LittleEndianWrite(const uint32_t &, uint8_t *);
void LittleEndianRead(const uint8_t *, uint32_t &);

auto SocketExactRead(ip::tcp::socket &, std::vector<uint8_t> &) -> ErrorKind;
auto SocketExactWrite(const std::vector<uint8_t> &, ip::tcp::socket &) -> ErrorKind;
