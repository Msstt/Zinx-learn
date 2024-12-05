#pragma once
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::system;

class IConnection {
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual auto GetConnectionId() -> uint32_t = 0;
  virtual auto RemoteAddress() -> ip::tcp::endpoint = 0;
};

typedef auto HandleFunction(ip::tcp::socket &, char *, int) -> bool;