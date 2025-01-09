#pragma once
#include "core/WorldManager.h"
#include "ziface/IRouter.h"

class Move : public IRouter {
 public:
  void PreHandle(IRequest &) override;
  void Handle(IRequest &) override;
  void PostHandle(IRequest &) override;

 private:
};