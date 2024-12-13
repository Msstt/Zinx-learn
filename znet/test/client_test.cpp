/*
  回显服务器测试
*/
#include <cstring>
#include <thread>

#include "znet/Message.h"
#include "znet/Router.h"
#include "znet/Server.h"

auto InitSocket(ip::tcp::socket &client, std::string ip, uint16_t port)
    -> bool {
  error_code err;
  auto endpoint = ip::tcp::endpoint(ip::address::from_string(ip, err), port);
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

int main() {
  std::string data = "hello ZINX";
  Message msg;
  msg.SetDataLen(data.size());
  for (size_t i = 0; i < data.size(); i++) {
    msg.GetData()[i] = static_cast<uint8_t>(data[i]);
  }

  auto server = NewServer();
  server->AddRouter(std::make_shared<Router>());

  CREATE_THREAD {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    io_service service;
    ip::tcp::socket client(service);
    if (!InitSocket(client, "127.0.0.1", 7777)) {
      return;
    }
    error_code err;
    uint32_t msg_id = 0;
    while (true) {
      msg.SetId(msg_id++);
      if (!Connection::SendMsg(client, msg)) {
        LOG(ERROR) << "client send data failed: " << err;
        break;
      }
      Message recv_msg;
      if (!Connection::RecvMsg(client, recv_msg)) {
        LOG(ERROR) << "client recv data failed: " << err;
        break;
      }
      std::string recv_data;
      for (const auto &c : recv_msg.GetData()) {
        recv_data += static_cast<char>(c);
      }
      LOG(INFO) << " server call back : " << recv_data
                << ", cnt = " << recv_msg.GetDataLen();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    client.close(err);
    if (err) {
      LOG(ERROR) << "client close failed: " << err;
    }
  }
  CREATE_THREAD_

  server->Serve();
  while (true) {
  }
}