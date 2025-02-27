#pragma once
#include "utils/Prelude.h"
#include "ziface/IRouter.h"

class EchoRouter : public IRouter {
 public:
  void PreHandle(IRequest &request) override;
  void Handle(IRequest &request) override;
  void PostHandle(IRequest &request) override;
};