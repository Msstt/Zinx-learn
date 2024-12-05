#include "znet/Server.h"
#include <cstring>
#include <thread>

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
  auto server = NewServer("[zinx V0.1]");

  CREATE_THREAD {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    io_service service;
    ip::tcp::socket client(service);
    if (!InitSocket(client, "127.0.0.1", 7777)) {
      return;
    }
    char buf[BUFFER_SIZE];
    error_code err;
    while (true) {
      client.write_some(buffer("hello ZINX"), err);
      if (err) {
        LOG(ERROR) << "client send data failed: " << err;
        break;
      }
      int bytes = client.read_some(buffer(buf, BUFFER_SIZE), err);
      if (err) {
        LOG(ERROR) << "client recv data failed: " << err;
        break;
      }
      LOG(INFO) << " server call back : " << buf << ", cnt = " << bytes - 1;
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