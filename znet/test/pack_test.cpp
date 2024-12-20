/*
  拆包解包测试
*/
#include <boost/asio.hpp>
#include <thread>

#include "znet/Connection.h"
#include "znet/Message.h"

using namespace boost::asio;
using namespace boost::system;

void Server() {
  io_service service;
  auto acceptor = ip::tcp::acceptor(
      service, ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 7777));
  ip::tcp::socket client(service);
  acceptor.accept(client);
  DataPack pack;
  while (true) {
    Message msg;
    if (Connection::RecvMsg(client, msg) != ErrorKind::OK_) {
      LOG(ERROR) << "RecvMsg failed";
      return;
    }

    std::string data;
    for (const auto &c : msg.GetData()) {
      data += static_cast<char>(c);
    }
    LOG(INFO) << "==> Recv Msg: ID=" << msg.GetId()
              << ", len=" << msg.GetDataLen() << ", data=\"" << data << "\"";
  }
}

void Client() {
  io_service service;
  ip::tcp::socket client(service);
  client.connect(
      ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 7777));
  DataPack pack;
  uint32_t id = 0;
  std::vector<std::string> data = {"hello", "ZINX"};
  while (true) {
    std::vector<uint8_t> buf;
    for (size_t i = 0; i < data.size(); i++) {
      Message msg;
      msg.SetId(id++);
      msg.SetDataLen(data[i].size());
      for (size_t j = 0; j < data[i].size(); j++) {
        msg.GetData()[j] = static_cast<uint8_t>(data[i][j]);
      }
      std::vector<uint8_t> tmp_buf;
      pack.Pack(msg, tmp_buf);
      buf.insert(buf.end(), tmp_buf.begin(), tmp_buf.end());
    }
    if (SocketExactWrite(buf, client) != ErrorKind::OK_) {
      return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int main() {
  std::thread server(Server);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::thread client(Client);
  server.join();
  client.join();
  return 0;
}