#pragma once
#include "utils/Prelude.h"
#include "ziface/IRouter.h"

class IMsgHandle {
 public:
  virtual ~IMsgHandle() = default;

  virtual void AddRouter(uint32_t, std::shared_ptr<IRouter>) = 0;
  virtual void DoMsgHandler(IRequest &) = 0;
  virtual void StartWorkerPool() = 0;
  virtual void SendMsgToTaskQueue(std::shared_ptr<IRequest>) = 0;
};