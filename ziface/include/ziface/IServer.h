#pragma once
#include "ziface/IRouter.h"
#include "ziface/IConnManager.h"

class IServer {
public:
  virtual ~IServer() = default;

  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual void Serve() = 0;

  virtual void AddRouter(uint32_t, std::shared_ptr<IRouter>) = 0;
  virtual auto GetConnManager() -> IConnManager& = 0;

  virtual void SetOnConnectionStart(std::function<void(IConnection&)>) = 0;
  virtual void SetOnConnectionStop(std::function<void(IConnection&)>) = 0;
  virtual void CallOnConnectionStart(IConnection&) = 0;
  virtual void CallOnConnectionStop(IConnection&) = 0;
};