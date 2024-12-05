#include "znet/Connection.h"

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
      if (!this->handle_api_(this->socket_, this->buffer_, bytes)) {
        LOG(ERROR) << "Connection handle client data failed: " << err;
        break;
      }
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

auto Connection::GetConnectionId() -> uint32_t { return this->connection_id_; }

auto Connection::RemoteAddress() -> ip::tcp::endpoint {
  return this->socket_.remote_endpoint();
}