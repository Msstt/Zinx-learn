#include "znet/Connection.h"
#include "znet/Request.h"

Connection::~Connection() { this->Stop(); }

void Connection::Start() {
  CREATE_THREAD {
    DLOG(INFO) << "Connection start";
    error_code err;
    while (true) {
      Message msg;
      this->RecvMsg(this->socket_, msg);

      auto request = std::make_shared<Request>(*this, std::move(msg));
      CREATE_THREAD__(request) {
        this->msg_handle_.DoMsgHandler(*request);
      }
      CREATE_THREAD_
    }
    DLOG(INFO) << "Connection end";
    this->~Connection();
  }
  CREATE_THREAD_
}

void Connection::Stop() { this->socket_.close(); }

auto Connection::GetConnectionId() const -> uint32_t {
  return this->connection_id_;
}

auto Connection::GetTCPConnection() -> ip::tcp::socket & {
  return this->socket_;
}

auto Connection::RemoteAddress() const -> ip::tcp::endpoint {
  return this->socket_.remote_endpoint();
}

auto Connection::SendMsg(const IMessage &msg) -> bool {
  return Connection::SendMsg(this->socket_, msg);
}

auto Connection::SendMsg(ip::tcp::socket &socket, const IMessage &msg) -> bool {
  DataPack pack;
  std::vector<uint8_t> buf;
  pack.Pack(msg, buf);
  if (!SocketExactWrite(buf, socket)) {
    return false;
  }
  return true;
}

auto Connection::RecvMsg(ip::tcp::socket &socket, IMessage &msg) -> bool {
  DataPack pack;
  std::vector<uint8_t> buf(pack.GetHeadLen());
  if (!SocketExactRead(socket, buf)) {
    return false;
  }
  pack.UnPack(buf, msg);
  if (!SocketExactRead(socket, msg.GetData())) {
    return false;
  }
  return true;
}