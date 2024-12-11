#include "znet/Router.h"

void Router::PreHandle(IRequest &request) {}

void Router::Handle(IRequest &request) {
  auto &socket = request.GetConnection().GetTCPConnection();
  error_code err;
  socket.write_some(buffer(request.GetData()), err);
  if (err) {
    LOG(ERROR) << "Router send data failed";
  }
}

void Router::PostHandle(IRequest &request) {}