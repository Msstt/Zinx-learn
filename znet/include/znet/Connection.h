#pragma once
#include <map>
#include <shared_mutex>

#include "utils/Prelude.h"
#include "ziface/IConnection.h"
#include "ziface/IMsgHandle.h"
#include "ziface/IServer.h"
#include "znet/DataPack.h"

#define BUFFER_SIZE 1024

class Connection : public IConnection,
                   public std::enable_shared_from_this<Connection> {
 public:
  Connection(ip::tcp::socket socket, uint32_t connection_id,
             IMsgHandle &msg_handle, IServer &server)
      : socket_(std::move(socket)),
        connection_id_(connection_id),
        msg_handle_(msg_handle),
        buffer_(GlobalObject::Instance().max_data_buffer_size_),
        server_(server) {}
  ~Connection();
  void Start() override;
  void Stop() override;
  auto GetConnectionId() const -> uint32_t override;
  auto GetTCPConnection() -> ip::tcp::socket & override;
  auto RemoteAddress() const -> ip::tcp::endpoint override;
  auto SendMsg(const IMessage &) -> ErrorKind override;
  auto RecvMsg(IMessage &) -> ErrorKind;

  static auto SendMsg(ip::tcp::socket &, const IMessage &) -> ErrorKind;
  static auto RecvMsg(ip::tcp::socket &, IMessage &) -> ErrorKind;

  void SetProperty(const std::string &, std::shared_ptr<void>) override;
  auto GetProperty(const std::string &) const -> std::shared_ptr<void> override;
  void RemoveProperty(const std::string &) override;

 private:
  void StartReader();
  void StartWriter();

  std::atomic<bool> is_close_{false};
  std::mutex socket_mutex_{};
  ip::tcp::socket socket_;
  uint32_t connection_id_;
  IMsgHandle &msg_handle_;
  SyncQueue<uint8_t> buffer_;
  IServer &server_;

  mutable std::shared_mutex properties_mutex_{};
  std::map<std::string, std::shared_ptr<void>> properties_{};

  ThreadPool handlers_{};
};
