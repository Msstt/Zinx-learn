#pragma once
#include "utils/prelude.h"
#include "ziface/IConnection.h"
#include "ziface/IMsgHandle.h"
#include "znet/DataPack.h"

#define BUFFER_SIZE 1024

class Connection : public IConnection {
public:
  Connection(ip::tcp::socket socket, uint32_t connection_id,
             IMsgHandle &msg_handle)
      : socket_(std::move(socket)), connection_id_(connection_id),
        msg_handle_(msg_handle) {}
  ~Connection();
  void Start() override;
  void Stop() override;
  auto GetConnectionId() const -> uint32_t override;
  auto GetTCPConnection() -> ip::tcp::socket & override;
  auto RemoteAddress() const -> ip::tcp::endpoint override;
  auto SendMsg(const IMessage &) -> bool override;

  static auto SendMsg(ip::tcp::socket &, const IMessage &) -> bool;
  static auto RecvMsg(ip::tcp::socket &, IMessage &) -> bool;

private:
  ip::tcp::socket socket_;
  uint32_t connection_id_;
  IMsgHandle &msg_handle_;
  uint8_t buffer_[BUFFER_SIZE];
};