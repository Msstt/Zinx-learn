#include "znet/EchoRouter.h"

void EchoRouter::PreHandle(IRequest &request) {}

void EchoRouter::Handle(IRequest &request) {
  request.GetConnection().SendMsg(request.GetMsg());
}

void EchoRouter::PostHandle(IRequest &request) {}