/*
  回显服务器测试 - 客户端
*/
#include <cstring>
#include <thread>

#include "znet/Connection.h"
#include "znet/Message.h"

auto InitSocket(ip::tcp::socket& client, std::string ip, uint16_t port)
    -> bool {
  error_code err;
  auto endpoint = ip::tcp::endpoint(ip::make_address(ip, err), port);
  if (err) {
    LOG(ERROR) << "client get ip fail: " << err;
    return false;
  }
  client.connect(endpoint, err);
  if (err) {
    LOG(ERROR) << "client connect fail: " << err;
    return false;
  }
  return true;
}

std::atomic<bool> is_running = true;

void Stop(const boost::system::error_code& error, int signal_number) {
  is_running = false;
}

auto ReadMsg(ip::tcp::socket& client) -> bool {
  error_code err;
  Message recv_msg;
  if (Connection::RecvMsg(client, recv_msg) != ErrorKind::OK_) {
    LOG(ERROR) << "client recv data failed: " << err;
    return false;
  }
  LOG(INFO) << "==> Recv Msg: ID= " << recv_msg.GetId()
            << " , len= " << recv_msg.GetDataLen()
            << " , data= " << recv_msg.ToString();
  return true;
}

int main(int argc, char* argv[]) {
  std::string data;
  getline(std::cin, data);
  Message msg(0, data);

  std::thread client([&]() {
    io_context service;
    ip::tcp::socket client(service);
    if (!InitSocket(client, "127.0.0.1", 7777)) {
      LOG(ERROR) << "client connect failed!";
      return;
    }
    LOG(INFO) << "client connect success...";
    if (!ReadMsg(client)) {
      return;
    }
    error_code err;
    uint32_t msg_id = 0;
    while (is_running) {
      msg.SetId(msg_id ^= 1);
      if (Connection::SendMsg(client, msg) != ErrorKind::OK_) {
        LOG(ERROR) << "client send data failed: " << err;
        break;
      }
      if (!ReadMsg(client)) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    client.close(err);
    if (err) {
      LOG(ERROR) << "client close failed: " << err;
    } else {
      LOG(INFO) << "client close success...";
    }
  });

  io_context service;
  boost::asio::signal_set signals(service, SIGINT, SIGTERM);
  signals.async_wait(&Stop);
  service.run();

  client.join();
}