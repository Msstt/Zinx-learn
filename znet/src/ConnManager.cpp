#include "znet/ConnManager.h"

void ConnManager::Add(std::shared_ptr<IConnection> connection) {
  std::unique_lock lock(this->mutex_);
  this->connections_[connection->GetConnectionId()] = connection;
}

void ConnManager::Remove(std::shared_ptr<IConnection> connection) {
  std::unique_lock lock(this->mutex_);
  this->connections_.erase(connection->GetConnectionId());
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