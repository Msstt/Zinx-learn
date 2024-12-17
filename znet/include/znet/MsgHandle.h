#pragma once
#include "ziface/IMsgHandle.h"

class MsgHandle : public IMsgHandle {
public:
  void AddRouter(uint32_t, std::shared_ptr<IRouter>) override;
  void DoMsgHandler(IRequest &) override;

private:
  std::map<uint32_t, std::shared_ptr<IRouter>> apis_;
};