/*
  回显服务器测试 - 服务端
*/

#include "znet/EchoRouter.h"
#include "znet/Message.h"
#include "znet/Server.h"
#include "znet/UpperRouter.h"

void Stop(const boost::system::error_code& error, int signal_number) {}

void DoStart(IConnection& connection) {
  LOG(INFO) << "Call start function";
  connection.SendMsg(Message(2, "Do connection start..."));
}

void DoStop(IConnection& connection) {
  LOG(INFO) << "Call stop function";
}

int main(int argc, char* argv[]) {
  auto server = NewServer();
  server->AddRouter(0, std::make_shared<EchoRouter>());
  server->AddRouter(1, std::make_shared<UpperRouter>());
  server->SetOnConnectionStart(DoStart);
  server->SetOnConnectionStop(DoStop);
  server->Start();

  boost::asio::io_service io_service;
  boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
  signals.async_wait(&Stop);
  io_service.run();

  server->Stop();
}