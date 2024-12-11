#pragma once
#include "utils/prelude.h"
#include "ziface/IRouter.h"

class Router : public IRouter {
public:
  void PreHandle(IRequest &request) override;
  void Handle(IRequest &request) override;
  void PostHandle(IRequest &request) override;
};