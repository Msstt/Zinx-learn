#pragma once
#include "utils/Prelude.h"
#include "ziface/IConnection.h"

class IConnManager {
 public:
  virtual ~IConnManager() = default;

  virtual void Add(std::shared_ptr<IConnection>) = 0;
  virtual void Remove(std::shared_ptr<IConnection>) = 0;
  virtual auto Get(uint32_t) -> std::shared_ptr<IConnection> = 0;
  virtual auto GetConnectionCount() -> uint32_t = 0;
  virtual void ClearConnection() = 0;
};