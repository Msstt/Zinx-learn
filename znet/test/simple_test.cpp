/*
  回显服务器测试
*/
#include <cstring>
#include <thread>

#include "znet/EchoRouter.h"
#include "znet/Message.h"
#include "znet/Server.h"
#include "znet/UpperRouter.h"

auto InitSocket(ip::tcp::socket& client, std::string ip, uint16_t port)
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

int main(int argc, char* argv[]) {
  auto server = NewServer();
  server->AddRouter(0, std::make_shared<EchoRouter>());
  server->AddRouter(1, std::make_shared<UpperRouter>());
  server->Start();
  std::this_thread::sleep_for(std::chrono::seconds(3));

  auto client = [&](std::string data, int cnt) {
    Message msg(0, data);

    io_service service;
    ip::tcp::socket client(service);
    if (!InitSocket(client, "127.0.0.1", 7777)) {
      return;
    }
    error_code err;
    uint32_t msg_id = 0;
    while (cnt--) {
      msg.SetId(msg_id ^= 1);
      if (Connection::SendMsg(client, msg) != ErrorKind::OK_) {
        LOG(ERROR) << "client send data failed: " << err;
        break;
      }
      Message recv_msg;
      if (Connection::RecvMsg(client, recv_msg) != ErrorKind::OK_) {
        LOG(ERROR) << "client recv data failed: " << err;
        break;
      }
      LOG(INFO) << " server call back : " << recv_msg.ToString()
                << ", cnt = " << recv_msg.GetDataLen();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    client.close(err);
    if (err) {
      LOG(ERROR) << "client close failed: " << err;
    }
  };
  std::vector<std::thread> threads;
  threads.emplace_back(client, "hello ZINX", std::numeric_limits<int>::max());
  std::vector<std::string> datas = {"ooOooO", "19 10", "ad10 - 1", "?.,[]"};
  for (const auto& data : datas) {
    threads.emplace_back(client, data, 10);
  }
  for (auto& thread : threads) {
    thread.join();
  }
}