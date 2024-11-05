#include "znet/Server.h"
#include <cstring>

auto GetClientSocket(std::string ip, uint16_t port) -> int {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ip.c_str());
  address.sin_port = htons(port);
  if (connect(socket_fd, reinterpret_cast<sockaddr *>(&address),
              sizeof(address)) == -1) {
    std::cerr << "connect server fail" << std::endl;
    return -1;
  }
  return socket_fd;
}

int main() {
  auto server = NewServer("[zinx V0.1]");
  auto future = std::async(std::launch::async, [&]() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    int client_fd = GetClientSocket("127.0.0.1", 7777);
    if (client_fd == -1) {
      return;
    }
    char buffer[BUFFER_SIZE];
    while (true) {
      strcpy(buffer, "hello ZINX");
      if (write(client_fd, buffer, 10) <= 0) {
        std::cerr << "send data failed" << std::endl;
        break;
      }
      int data_cnt;
      if ((data_cnt = read(client_fd, buffer, BUFFER_SIZE)) <= 0) {
        std::cerr << "recv data failed" << std::endl;
        break;
      }
      buffer[data_cnt] = '\0';
      std::cout << " server call back : " << buffer << ", cnt = " << data_cnt
                << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    close(client_fd);
  });
  server->Serve();
  while (true) {
  }
}