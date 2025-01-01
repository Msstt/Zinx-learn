#pragma once
#include "utils/Prelude.h"
#include "ziface/IServer.h"
#include "znet/ConnManager.h"
#include "znet/Connection.h"
#include "znet/MsgHandle.h"

class Server : public IServer {
 public:
  Server(std::string name, std::string ip_version, std::string ip,
         uint16_t port)
      : name_(name), ip_version_(ip_version), ip_(ip), port_(port) {}

  void Start() override;
  void Stop() override;
  void Serve() override;

  void AddRouter(uint32_t, std::shared_ptr<IRouter>) override;
  auto GetConnManager() -> IConnManager& override;

  void SetOnConnectionStart(std::function<void(IConnection&)>) override;
  void SetOnConnectionStop(std::function<void(IConnection&)>) override;
  void CallOnConnectionStart(IConnection&) override;
  void CallOnConnectionStop(IConnection&) override;

 private:
  // 服务器基础属性
  std::string name_;
  std::string ip_version_;
  std::string ip_;
  uint16_t port_;

  std::atomic<bool> is_close_{false};

  MsgHandle msg_handle_{};      // 路由分发
  ConnManager conn_manager_{};  // 连接管理

  // 回调函数
  std::function<void(IConnection&)> on_connection_start_{};
  std::function<void(IConnection&)> on_connection_stop_{};
};

auto NewServer() -> std::unique_ptr<IServer>;