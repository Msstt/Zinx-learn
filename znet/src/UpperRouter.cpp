#include "znet/UpperRouter.h"
#include "znet/Message.h"

void UpperRouter::PreHandle(IRequest &request) {}

void UpperRouter::Handle(IRequest &request) {
  Message answer;
  answer.SetId(request.GetMsg().GetId());
  answer.SetDataLen(request.GetMsg().GetDataLen());
  for (size_t i = 0; i < answer.GetDataLen(); i++) {
    answer.GetData()[i] = static_cast<uint8_t>(
        std::toupper(static_cast<char>(request.GetMsg().GetData()[i])));
  }
  request.GetConnection().SendMsg(answer);
}

void UpperRouter::PostHandle(IRequest &request) {}