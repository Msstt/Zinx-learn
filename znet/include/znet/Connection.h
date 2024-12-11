#pragma once
#include "tools/prelude.h"
#include "ziface/IConnection.h"
#include "ziface/IRouter.h"

#define BUFFER_SIZE 1024

class Connection : public IConnection {
public:
  Connection(ip::tcp::socket socket, uint32_t connection_id, IRouter &router)
      : socket_(std::move(socket)), connection_id_(connection_id),
        router_(router) {}
  ~Connection();
  void Start() override;
  void Stop() override;
  auto GetConnectionId() const -> uint32_t override;
  auto GetTCPConnection() -> ip::tcp::socket & override;
  auto RemoteAddress() const -> ip::tcp::endpoint override;

private:
  ip::tcp::socket socket_;
  uint32_t connection_id_;
  std::atomic<bool> is_closed_{false};
  IRouter &router_;
  char buffer_[BUFFER_SIZE];
};