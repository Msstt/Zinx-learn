#include "znet/Router.h"

void Router::PreHandle(IRequest &request) {}

void Router::Handle(IRequest &request) {
  request.GetConnection().SendMsg(request.GetMsg());
}

void Router::PostHandle(IRequest &request) {}