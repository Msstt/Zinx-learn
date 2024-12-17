#pragma once
#include "utils/prelude.h"
#include "ziface/IRouter.h"

class IMsgHandle {
public:
  virtual ~IMsgHandle() = default;

  virtual void AddRouter(uint32_t, std::shared_ptr<IRouter>) = 0;
  virtual void DoMsgHandler(IRequest &) = 0;
};