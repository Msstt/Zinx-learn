#pragma once
#include "ziface/IRouter.h"
#include "znet/Message.h"
#include "core/WorldManager.h"

class WorldChatAPI : public IRouter {
 public:
  void PreHandle(IRequest &) override;
  void Handle(IRequest &) override;
  void PostHandle(IRequest &) override;
};