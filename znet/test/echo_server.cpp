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

  connection.SetProperty("name", std::make_shared<std::string>("Zinx"));
}

void DoStop(IConnection& connection) {
  LOG(INFO) << "Call stop function";

  std::shared_ptr<void> value = connection.GetProperty("name");
  if (!value) {
    LOG(ERROR) << "Get property failed";
    exit(-1);
  }
  if (*std::static_pointer_cast<std::string>(value) != "Zinx") {
    LOG(ERROR) << "Get property error";
    exit(-1);
  }
}

int main(int argc, char* argv[]) {
  GlobalObject::Instance().LoadTestConfig("test");
  auto server = NewServer();
  server->AddRouter(0, std::make_shared<EchoRouter>());
  server->AddRouter(1, std::make_shared<UpperRouter>());
  server->SetOnConnectionStart(DoStart);
  server->SetOnConnectionStop(DoStop);
  server->Start();

  boost::asio::io_context io_context;
  boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
  signals.async_wait(&Stop);
  io_context.run();

  server->Stop();
}