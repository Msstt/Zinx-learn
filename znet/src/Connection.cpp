#include "znet/Connection.h"

#include "znet/Request.h"

Connection::~Connection() { this->Stop(); }

void Connection::Start() {
  this->server_.CallOnConnectionStart(*this);

  this->StartReader();
  this->StartWriter();
}

void Connection::StartReader() {
  auto use_pool = GlobalObject::Instance().worker_pool_size_ != 0;

  auto self = this->shared_from_this();
  this->server_.GetConnManager().AddReaderThread(this->connection_id_, [=] {
    DLOG(INFO) << "[Connection reader start]";
    error_code err;
    while (!self->is_close_) {
      Message msg;
      auto ret = self->RecvMsg(self->socket_, msg);
      if (ret != ErrorKind::OK_) {
        if (ret != ErrorKind::EOF_) {
          LOG(ERROR) << "Connection read socket failed";
        }
        self->Stop();
        break;
      }

      auto request = std::make_shared<Request>(*self, std::move(msg));
      if (use_pool) {
        self->msg_handle_.SendMsgToTaskQueue(request);
      } else {
        self->handlers_.AddThread(
            [&, request]() { self->msg_handle_.DoMsgHandler(*request);
            });
      }
    }
    DLOG(INFO) << "[Connection reader end]";
  });
}

void Connection::StartWriter() {
  auto self = this->shared_from_this();
  this->server_.GetConnManager().AddWriterThread(this->connection_id_, [=] {
    DLOG(INFO) << "[Connection writer start]";
    while (!self->is_close_) {
      auto data = self->buffer_.PopAll();
      if (data.empty()) {
        continue;
      }

      std::unique_lock<std::mutex> lock(self->socket_mutex_);
      auto ret = SocketExactWrite(data, self->socket_);
      if (ret != ErrorKind::OK_) {
        if (ret != ErrorKind::EOF_) {
          LOG(ERROR) << "Connection write socket failed";
        }
        self->Stop();
        break;
      }
    }
    DLOG(INFO) << "[Connection writer end]";
  });
}

void Connection::Stop() {
  if (this->is_close_) {
    return;
  }
  this->is_close_ = true;

  this->server_.CallOnConnectionStop(*this);

  this->buffer_.Finish();
  error_code err;
  this->socket_.close(err);
  if (err) {
    LOG(ERROR) << "Socket close failed";
  }
  this->server_.GetConnManager().Remove(this->shared_from_this());
}

auto Connection::GetConnectionId() const -> uint32_t {
  return this->connection_id_;
}

auto Connection::GetTCPConnection() -> ip::tcp::socket & {
  return this->socket_;
}

auto Connection::RemoteAddress() const -> ip::tcp::endpoint {
  return this->socket_.remote_endpoint();
}

auto Connection::SendMsg(const IMessage &msg) -> ErrorKind {
  DataPack pack;
  std::vector<uint8_t> buf;
  pack.Pack(msg, buf);
  this->buffer_.Push(buf);
  return ErrorKind::OK_;
}

auto Connection::SendMsg(ip::tcp::socket &socket, const IMessage &msg)
    -> ErrorKind {
  DataPack pack;
  std::vector<uint8_t> buf;
  pack.Pack(msg, buf);
  return SocketExactWrite(buf, socket);
}

auto Connection::RecvMsg(IMessage &msg) -> ErrorKind {
  std::unique_lock<std::mutex> lock(this->socket_mutex_);
  DataPack pack;
  std::vector<uint8_t> buf(pack.GetHeadLen());
  auto ret = SocketExactRead(this->socket_, buf);
  if (ret != ErrorKind::OK_) {
    return ret;
  }
  pack.UnPack(buf, msg);
  if (msg.GetDataLen() == 0) {
    return ErrorKind::OK_;
  }
  return SocketExactRead(this->socket_, msg.GetData());
}

auto Connection::RecvMsg(ip::tcp::socket &socket, IMessage &msg) -> ErrorKind {
  DataPack pack;
  std::vector<uint8_t> buf(pack.GetHeadLen());
  auto ret = SocketExactRead(socket, buf);
  if (ret != ErrorKind::OK_) {
    return ret;
  }
  pack.UnPack(buf, msg);
  if (msg.GetDataLen() == 0) {
    return ErrorKind::OK_;
  }
  return SocketExactRead(socket, msg.GetData());
}

void Connection::SetProperty(const std::string &key,
                             std::shared_ptr<void> value) {
  std::unique_lock lock(this->properties_mutex_);
  this->properties_[key] = value;
}

auto Connection::GetProperty(const std::string &key) const
    -> std::shared_ptr<void> {
  std::shared_lock lock(this->properties_mutex_);
  if (this->properties_.find(key) == this->properties_.end()) {
    return nullptr;
  }
  return this->properties_.at(key);
}

void Connection::RemoveProperty(const std::string &key) {
  std::unique_lock lock(this->properties_mutex_);
  this->properties_.erase(key);
}