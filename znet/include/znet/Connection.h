#pragma once
#include "tools/prelude.h"
#include "ziface/IConnection.h"

#define BUFFER_SIZE 1024

class Connection : public IConnection {
public:
  Connection(ip::tcp::socket socket, uint32_t connection_id,
             HandleFunction handle_api)
      : socket_(std::move(socket)), connection_id_(connection_id),
        handle_api_(handle_api) {}
  ~Connection();
  void Start() override;
  void Stop() override;
  auto GetConnectionId() -> uint32_t override;
  auto RemoteAddress() -> ip::tcp::endpoint override;

private:
  ip::tcp::socket socket_;
  uint32_t connection_id_;
  std::atomic<bool> is_closed_{false};
  HandleFunction *handle_api_;
  char buffer_[BUFFER_SIZE];
};