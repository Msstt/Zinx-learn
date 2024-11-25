#pragma once
#include <boost/asio.hpp>
#include <iostream>

#include "ziface/IServer.h"

using namespace boost::asio;
using namespace boost::system;

#define BUFFER_SIZE 1024

class Server : public IServer {
public:
  Server(std::string name, std::string ip_version, std::string ip,
         uint16_t port)
      : name_(name), ip_version_(ip_version), ip_(ip), port_(port) {}
  void Start() override;
  void Stop() override;
  void Serve() override;

private:
  std::string name_;
  std::string ip_version_;
  std::string ip_;
  uint16_t port_;
};

auto NewServer(std::string) -> std::unique_ptr<IServer>;