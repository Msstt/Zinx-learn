#pragma once
#include <shared_mutex>

#include "ziface/IConnManager.h"

class ConnManager : public IConnManager {
 public:
  void Add(std::shared_ptr<IConnection>) override;
  void Remove(std::shared_ptr<IConnection>) override;
  auto Get(uint32_t) -> std::shared_ptr<IConnection> override;
  auto GetConnectionCount() -> uint32_t override;
  void ClearConnection() override;

 private:
  std::map<uint32_t, std::shared_ptr<IConnection>> connections_{};
  std::shared_mutex mutex_{};
};