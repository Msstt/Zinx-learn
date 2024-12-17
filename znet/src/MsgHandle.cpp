#include "znet/MsgHandle.h"

void MsgHandle::AddRouter(uint32_t msg_id, std::shared_ptr<IRouter> router) {
  if (this->apis_.count(msg_id)) {
    LOG(WARNING) << "Repeated api , msgId = " << msg_id;
    return;
  }
  this->apis_[msg_id] = router;
}

void MsgHandle::DoMsgHandler(IRequest &request) {
  if (!this->apis_.count(request.GetMsg().GetId())) {
    LOG(ERROR) << "api msgId = " << request.GetMsg().GetId()
               << " is not found!";
  }
  auto &router = *this->apis_[request.GetMsg().GetId()];
  router.PreHandle(request);
  router.Handle(request);
  router.PostHandle(request);
}