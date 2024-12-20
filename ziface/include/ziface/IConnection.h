#pragma once
#include "utils/prelude.h"
#include "ziface/IMessage.h"

class IConnection {
public:
  virtual ~IConnection() = default;

  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual auto GetConnectionId() const -> uint32_t = 0;
  virtual auto GetTCPConnection() -> ip::tcp::socket & = 0;
  virtual auto RemoteAddress() const -> ip::tcp::endpoint = 0;
  virtual auto SendMsg(const IMessage &) -> ErrorKind = 0;
};