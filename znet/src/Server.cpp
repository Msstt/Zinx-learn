#include "znet/Server.h"

auto GetListenSocket(std::string ip, uint16_t port) -> int {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    std::cerr << "get listen socket call socket failed" << std::endl;
    return -1;
  }
  sockaddr_in address;
  int address_len = sizeof(address);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ip.c_str());
  address.sin_port = htons(port);
  if (bind(fd, reinterpret_cast<sockaddr *>(&address), address_len) == -1) {
    std::cerr << "get listen socket call bind failed" << std::endl;
    return -1;
  }
  if (listen(fd, 5) == -1) {
    std::cerr << "get listen socket call listen failed" << std::endl;
    return -1;
  }
  return fd;
}

auto GetClientSocket(int server_fd) -> int {
  sockaddr_in address;
  int address_len = sizeof(address);
  int fd = accept(server_fd, reinterpret_cast<sockaddr *>(&address),
                  reinterpret_cast<socklen_t *>(&address_len));
  if (fd == -1) {
    std::cerr << "get client socket call accept failed" << std::endl;
    return -1;
  }
  std::cout << "Server connect " << inet_ntoa(address.sin_addr) << " ..."
            << std::endl;
  return fd;
}

auto HandleClient(int client_fd) {
  char buffer[BUFFER_SIZE];
  while (true) {
    int data_size = read(client_fd, buffer, BUFFER_SIZE);
    if (data_size <= 0) {
      break;
    }
    write(client_fd, buffer, data_size);
  }
  close(client_fd);
}

void Server::Start() {
  std::cout << "[START] Server listenner at IP: " << this->ip_ << ", Port "
            << this->port_ << ", is starting" << std::endl;
  auto future = std::async(std::launch::async, [&] {
    int server_fd = GetListenSocket(this->ip_, this->port_);
    if (server_fd == -1) {
      return;
    }
    std::cout << "start Zinx server " << this->name_
              << " success, now listenning..." << std::endl;
    while (true) {
      int client_fd = GetClientSocket(server_fd);
      if (client_fd == -1) {
        continue;
      }
      auto future = std::async(std::launch::async, HandleClient, client_fd);
    }
  });
}

void Server::Stop() {
  std::cout << "[STOP] Zinx server " << this->name_ << " ..." << std::endl;
}

void Server::Serve() {
  this->Start();
  while (true) {
  }
}

auto NewServer(std::string name) -> std::unique_ptr<IServer> {
  std::unique_ptr<IServer> server =
      std::make_unique<Server>(name, "tcp4", "0.0.0.0", 7777);
  return server;
}