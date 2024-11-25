#include "znet/Server.h"
#include <cstring>

auto InitSocket(ip::tcp::socket &client, std::string ip, uint16_t port)
    -> bool {
  error_code err;
  auto endpoint = ip::tcp::endpoint(ip::address::from_string(ip, err), port);
  if (err) {
    std::cerr << "client get ip fail: " << err << std::endl;
    return false;
  }
  client.connect(endpoint, err);
  if (err) {
    std::cerr << "client connect fail: " << err << std::endl;
    return false;
  }
  return true;
}

int main() {
  auto server = NewServer("[zinx V0.1]");

  auto future = std::async(std::launch::async, [&]() {
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
        std::cerr << "client send data failed: " << err << std::endl;
        break;
      }
      int bytes = client.read_some(buffer(buf, BUFFER_SIZE), err);
      if (err) {
        std::cerr << "client recv data failed: " << err << std::endl;
        break;
      }
      std::cout << " server call back : " << buf << ", cnt = " << bytes - 1
                << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    client.close(err);
    if (err) {
      std::cerr << "client close failed: " << err << std::endl;
    }
  });

  server->Serve();
  while (true) {
  }
}