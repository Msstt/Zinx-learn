#pragma once
#include "utils/Prelude.h"
#include "ziface/IMsgHandle.h"

class MsgHandle : public IMsgHandle {
 public:
  MsgHandle();
  ~MsgHandle();
  void AddRouter(uint32_t, std::shared_ptr<IRouter>) override;
  void DoMsgHandler(IRequest &) override;
  void StartWorkerPool() override;
  void SendMsgToTaskQueue(std::shared_ptr<IRequest>) override;

 private:
  std::map<uint32_t, std::shared_ptr<IRouter>> apis_{};
  std::vector<SyncQueue<std::shared_ptr<IRequest>> *> task_queues_{};
  std::vector<std::thread> threads_{};
};