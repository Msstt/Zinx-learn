#include "znet/Server.h"

auto InitAccpetor(ip::tcp::acceptor &acceptor, std::string ip, uint16_t port)
    -> bool {
  error_code err;
  auto endpoint = ip::tcp::endpoint(ip::address::from_string(ip, err), port);
  if (err) {
    LOG(ERROR) << "Server get ip fail: " << err;
    return false;
  }
  acceptor.open(endpoint.protocol(), err);
  if (err) {
    LOG(ERROR) << "Server open acceptor fail: " << err;
    return false;
  }
  acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint, err);
  if (err) {
    LOG(ERROR) << "Server bind acceptor fail: " << err;
    return false;
  }
  acceptor.listen();
  return true;
}

auto HandleClient(ip::tcp::socket &socket, char *buf, int bytes) -> bool {
  error_code err;
  socket.write_some(buffer(buf, bytes), err);
  if (err) {
    LOG(ERROR) << "Server send client data fail: " << err;
    return false;
  }
  return true;
}

void Server::Start() {
  if (this->router_.empty()) {
    return;
  }
  LOG(INFO) << "[START] Server listenner at IP: " << this->ip_ << ", Port "
            << this->port_ << ", is starting";
  CREATE_THREAD {
    io_service service;
    auto acceptor = ip::tcp::acceptor(service);
    if (!InitAccpetor(acceptor, this->ip_, this->port_)) {
      return;
    }
    LOG(INFO) << "start Zinx server " << this->name_
              << " success, now listenning...";
    uint32_t connection_id = 0;
    while (true) {
      ip::tcp::socket client(service);
      error_code err;
      acceptor.accept(client, err);
      if (err) {
        LOG(ERROR) << "Server get client socket failed: " << err;
        continue;
      }
      auto connecion =
          new Connection(std::move(client), connection_id++, *this->router_[0]);
      connecion->Start();
    }
  }
  CREATE_THREAD_
}

void Server::Stop() {
  LOG(INFO) << "[STOP] Zinx server " << this->name_ << " ...";
}

void Server::Serve() {
  this->Start();
  while (true) {
  }
}

void Server::AddRouter(std::shared_ptr<IRouter> router) {
  this->router_.push_back(router);
}

auto NewServer() -> std::unique_ptr<IServer> {
  const auto &conf = GlobalObject::Instance();
  std::unique_ptr<IServer> server =
      std::make_unique<Server>(conf.name_, "tcp4", conf.host_, conf.port_);
  return server;
}