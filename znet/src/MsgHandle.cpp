#include "znet/MsgHandle.h"

MsgHandle::MsgHandle() {
  auto pool_size = GlobalObject::Instance().worker_pool_size_;
  auto task_length = GlobalObject::Instance().max_worker_task_length_;
  for (size_t i = 0; i < pool_size; i++) {
    this->task_queues_.emplace_back(
        new SyncQueue<std::shared_ptr<IRequest>>(task_length));
  }
}

MsgHandle::~MsgHandle() {
  for (size_t i = 0; i < this->task_queues_.size(); i++) {
    delete this->task_queues_[i];
  }
}

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

void MsgHandle::StartWorkerPool() {
  for (size_t i = 0; i < this->task_queues_.size(); i++) {
    this->threads_.emplace_back([&, i] {
      while (true) {
        auto request = this->task_queues_[i]->Pop();
        this->DoMsgHandler(*request);
      }
    });
  }
}

void MsgHandle::SendMsgToTaskQueue(std::shared_ptr<IRequest> request) {
  auto worker_id =
      request->GetConnection().GetConnectionId() % this->task_queues_.size();
  this->task_queues_[worker_id]->Push(request);
}