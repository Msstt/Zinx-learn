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
  acceptor.non_blocking(true);
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
  LOG(INFO) << "[START] Server listenner at IP: " << this->ip_ << ", Port "
            << this->port_ << ", is starting";
  this->msg_handle_.StartWorkerPool();
  CREATE_THREAD {
    io_service service;
    auto acceptor = ip::tcp::acceptor(service);
    if (!InitAccpetor(acceptor, this->ip_, this->port_)) {
      return;
    }
    LOG(INFO) << "start Zinx server " << this->name_
              << " success, now listenning...";
    uint32_t connection_id = 0;
    while (!this->is_close_) {
      ip::tcp::socket client(service);
      error_code err;
      acceptor.accept(client, err);
      if (err == boost::asio::error::would_block) {
        continue;
      }
      if (err) {
        LOG(ERROR) << "Server get client socket failed: " << err;
        continue;
      }
      if (this->conn_manager_.GetConnectionCount() >=
          GlobalObject::Instance().max_connection_number_) {
        client.close(err);
        LOG(WARNING) << "Server reached the maximum number of connections";
        continue;
      }
      auto connecion = std::make_shared<Connection>(
          std::move(client), connection_id++, this->msg_handle_, *this);
      // FIXME:可能 Stop 后继续添加连接
      this->conn_manager_.Add(connecion);
      connecion->Start();
    }
    DLOG(INFO) << "[Server thread stop]";
  }
  CREATE_THREAD_
}

void Server::Stop() {
  LOG(INFO) << "[STOP] Zinx server " << this->name_ << " ...";

  this->is_close_ = true;
  this->conn_manager_.ClearConnection();
}

void Server::Serve() {
  this->Start();
  while (true) {
  }
}

void Server::AddRouter(uint32_t msg_id, std::shared_ptr<IRouter> router) {
  this->msg_handle_.AddRouter(msg_id, router);
}

auto Server::GetConnManager() -> IConnManager & { return this->conn_manager_; }

void Server::SetOnConnectionStart(std::function<void(IConnection &)> function) {
  this->on_connection_start_ = function;
}

void Server::SetOnConnectionStop(std::function<void(IConnection &)> function) {
  this->on_connection_stop_ = function;
}

void Server::CallOnConnectionStart(IConnection &connection) {
  if (this->on_connection_start_ == nullptr) {
    LOG(WARNING) << "Server's on_connection_start_function has not set";
    return;
  }
  this->on_connection_start_(connection);
}

void Server::CallOnConnectionStop(IConnection &connection) {
  if (this->on_connection_stop_ == nullptr) {
    LOG(WARNING) << "Server's on_connection_stop_function has not set";
    return;
  }
  this->on_connection_stop_(connection);
}

auto NewServer() -> std::unique_ptr<IServer> {
  const auto &conf = GlobalObject::Instance();
  std::unique_ptr<IServer> server =
      std::make_unique<Server>(conf.name_, "tcp4", conf.host_, conf.port_);
  return server;
}