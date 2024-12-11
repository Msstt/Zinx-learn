#pragma once
#include "tools/prelude.h"
#include "ziface/IRequest.h"

class IRouter {
public:
  virtual ~IRouter() = default;
  virtual void PreHandle(IRequest &) {
    LOG(WARNING) << "Router PreHandle not implement";
  };
  virtual void Handle(IRequest &) {
    LOG(WARNING) << "Router Handle not implement";
  };
  virtual void PostHandle(IRequest &) {
    LOG(WARNING) << "Router PostHandle not implement";
  };
};