#include "znet/ConnManager.h"

ConnManager::ConnManager() {
  this->cleaner = std::make_unique<std::thread>([&]() {
    while (!this->is_close_) {
      auto connection_ids = this->del_connections_.PopAll();
      std::unique_lock lock(this->mutex_);
      for (const auto& connection_id : connection_ids) {
        this->readers_.at(connection_id)->join();
        this->readers_.erase(connection_id);
        this->writers_.at(connection_id)->join();
        this->writers_.erase(connection_id);
        DLOG(INFO) << "[Connection thread del]";
      }
    }
  });
}

ConnManager::~ConnManager() {
  this->is_close_ = true;
  this->del_connections_.Finish();
  this->cleaner->join();
}

void ConnManager::Add(std::shared_ptr<IConnection> connection) {
  std::unique_lock lock(this->mutex_);
  this->connections_[connection->GetConnectionId()] = connection;
}

void ConnManager::Remove(std::shared_ptr<IConnection> connection) {
  std::unique_lock lock(this->mutex_);
  this->connections_.erase(connection->GetConnectionId());
  this->del_connections_.Push(connection->GetConnectionId());
}

auto ConnManager::Get(uint32_t connection_id) -> std::shared_ptr<IConnection> {
  std::shared_lock lock(this->mutex_);
  if (this->connections_.find(connection_id) == this->connections_.end()) {
    return nullptr;
  } else {
    return this->connections_.at(connection_id);
  }
}

auto ConnManager::GetConnectionCount() -> uint32_t {
  std::shared_lock lock(this->mutex_);
  return this->connections_.size();
}

void ConnManager::ClearConnection() {
  std::unique_lock lock(this->mutex_);
  while (!this->connections_.empty()) {
    lock.unlock();
    this->connections_.begin()->second->Stop();
    lock.lock();
  }
}

void ConnManager::AddReaderThread(uint32_t connection_id,
                                  std::function<void()> func) {
  this->readers_[connection_id] = std::make_unique<std::thread>(func);
}

void ConnManager::AddWriterThread(uint32_t connection_id,
                                  std::function<void()> func) {
  this->writers_[connection_id] = std::make_unique<std::thread>(func);
}
