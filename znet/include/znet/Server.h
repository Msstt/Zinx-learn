#pragma once
#include "utils/Prelude.h"
#include "ziface/IServer.h"
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
  void AddRouter(uint32_t, std::shared_ptr<IRouter> router) override;

 private:
  std::string name_;
  std::string ip_version_;
  std::string ip_;
  uint16_t port_;
  MsgHandle msg_handle_;
};

auto NewServer() -> std::unique_ptr<IServer>;