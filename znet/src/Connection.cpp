#include "znet/Connection.h"
#include "znet/Request.h"

Connection::~Connection() { this->socket_.close(); }

void Connection::Start() {
  CREATE_THREAD {
    DLOG(INFO) << "Connection start";
    error_code err;
    while (!this->is_closed_) {
      size_t bytes =
          this->socket_.read_some(buffer(this->buffer_, BUFFER_SIZE), err);
      if (err) {
        LOG(ERROR) << "Connection read client data failed: " << err;
        break;
      }
      std::vector<uint8_t> data(bytes);
      for (size_t i = 0; i < bytes; i++) {
        data[i] = static_cast<uint8_t>(this->buffer_[i]);
      }
      auto request = std::make_shared<Request>(*this, std::move(data));
      CREATE_THREAD__(request) {
        this->router_.PreHandle(*request);
        this->router_.Handle(*request);
        this->router_.PostHandle(*request);
      }
      CREATE_THREAD_
    }
    DLOG(INFO) << "Connection end";
    this->~Connection();
  }
  CREATE_THREAD_
}

void Connection::Stop() {
  this->is_closed_ = false;
  this->socket_.cancel();
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