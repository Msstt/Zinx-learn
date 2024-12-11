#include "ziface/IRouter.h"
#pragma once

class IServer {
public:
  virtual ~IServer() = default;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual void Serve() = 0;
  virtual void AddRouter(std::shared_ptr<IRouter>) = 0;
};