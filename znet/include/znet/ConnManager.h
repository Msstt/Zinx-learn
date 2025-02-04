#pragma once
#include <map>
#include <shared_mutex>

#include "ziface/IConnManager.h"

class ConnManager : public IConnManager {
 public:
  ConnManager();
  ~ConnManager();

  void Add(std::shared_ptr<IConnection>) override;
  void Remove(std::shared_ptr<IConnection>) override;
  auto Get(uint32_t) -> std::shared_ptr<IConnection> override;
  auto GetConnectionCount() -> uint32_t override;
  void ClearConnection() override;

  void AddReaderThread(uint32_t, std::function<void()>) override;
  void AddWriterThread(uint32_t, std::function<void()>) override;

 private:
  std::map<uint32_t, std::shared_ptr<IConnection>> connections_{};
  std::map<uint32_t, std::unique_ptr<std::thread>> readers_{};
  std::map<uint32_t, std::unique_ptr<std::thread>> writers_{};
  SyncQueue<uint32_t> del_connections_{std::numeric_limits<uint32_t>::max()};
  std::shared_mutex mutex_{};
  std::unique_ptr<std::thread> cleaner;
  std::atomic<bool> is_close_{false};
};
