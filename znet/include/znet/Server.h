#pragma once
#include <boost/asio.hpp>

#include "tools/prelude.h"
#include "ziface/IServer.h"
#include "znet/Connection.h"

using namespace boost::asio;
using namespace boost::system;

class Server : public IServer {
public:
  Server(std::string name, std::string ip_version, std::string ip,
         uint16_t port)
      : name_(name), ip_version_(ip_version), ip_(ip), port_(port) {}
  void Start() override;
  void Stop() override;
  void Serve() override;
  void AddRouter(std::shared_ptr<IRouter> router) override;

private:
  std::string name_;
  std::string ip_version_;
  std::string ip_;
  uint16_t port_;
  std::vector<std::shared_ptr<IRouter>> router_;
};

auto NewServer(std::string) -> std::unique_ptr<IServer>;