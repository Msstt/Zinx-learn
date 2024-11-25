#include "znet/Server.h"

auto InitAccpetor(ip::tcp::acceptor &acceptor, std::string ip, uint16_t port)
    -> bool {
  error_code err;
  auto endpoint = ip::tcp::endpoint(ip::address::from_string(ip, err), port);
  if (err) {
    std::cerr << "server get ip fail: " << err << std::endl;
    return false;
  }
  acceptor.open(endpoint.protocol(), err);
  if (err) {
    std::cerr << "server open acceptor fail: " << err << std::endl;
    return false;
  }
  acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint, err);
  if (err) {
    std::cerr << "server bind acceptor fail: " << err << std::endl;
    return false;
  }
  acceptor.listen();
  return true;
}

void HandleClient(ip::tcp::socket &client) {
  char buf[BUFFER_SIZE];
  while (true) {
    error_code err;
    int bytes = client.read_some(buffer(buf, BUFFER_SIZE), err);
    if (err) {
      std::cerr << "server recv client data failed: " << err << std::endl;
      return;
    }
    client.write_some(buffer(buf, bytes), err);
    if (err) {
      std::cerr << "server send client data failed: " << err << std::endl;
      return;
    }
  }
}

void Server::Start() {
  std::cout << "[START] Server listenner at IP: " << this->ip_ << ", Port "
            << this->port_ << ", is starting" << std::endl;
  auto future = std::async(std::launch::async, [&] {
    io_service service;
    auto acceptor = ip::tcp::acceptor(service);
    if (!InitAccpetor(acceptor, this->ip_, this->port_)) {
      return;
    }
    std::cout << "start Zinx server " << this->name_
              << " success, now listenning..." << std::endl;
    while (true) {
      ip::tcp::socket client(service);
      error_code err;
      acceptor.accept(client, err);
      if (err) {
        std::cerr << "server get client socket failed: " << err << std::endl;
        continue;
      }
      auto future =
          std::async(std::launch::async, HandleClient, std::ref(client));
    }
  });
}

void Server::Stop() {
  std::cout << "[STOP] Zinx server " << this->name_ << " ..." << std::endl;
}

void Server::Serve() {
  this->Start();
  while (true) {
  }
}

auto NewServer(std::string name) -> std::unique_ptr<IServer> {
  std::unique_ptr<IServer> server =
      std::make_unique<Server>(name, "tcp4", "0.0.0.0", 7777);
  return server;
}