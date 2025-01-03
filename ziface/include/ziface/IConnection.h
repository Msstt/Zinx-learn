#pragma once
#include "utils/Prelude.h"
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

  virtual void SetProperty(std::string, std::shared_ptr<void>) = 0;
  virtual auto GetProperty(std::string, std::shared_ptr<void> &) const
      -> bool = 0;
  virtual void RemoveProperty(std::string) = 0;
};