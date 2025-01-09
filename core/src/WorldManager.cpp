#include "core/WorldManager.h"

auto WorldManager::Instance() -> WorldManager& {
  static WorldManager instance;
  return instance;
}

void WorldManager::AddPlayer(const std::shared_ptr<Player>& player) {
  std::unique_lock lock(this->players_mutex_);
  this->players_[player->GetId()] = player;
  this->aoi_.AddPlayerId(player->x_, player->z_, player->GetId());
}

void WorldManager::RemovePlayer(const size_t& player_id) {
  std::unique_lock lock(this->players_mutex_);
  if (this->players_.find(player_id) == this->players_.end()) {
    return;
  }
  auto player = this->players_.at(player_id);
  this->aoi_.RemovePlayerId(player->x_, player->z_, player->GetId());
  this->players_.erase(player_id);
}

auto WorldManager::GetPlayer(const size_t& player_id) const
    -> std::shared_ptr<Player> {
  std::shared_lock lock(this->players_mutex_);
  if (this->players_.find(player_id) == this->players_.end()) {
    return nullptr;
  }
  return this->players_.at(player_id);
}

auto WorldManager::GetAllPlayers() const
    -> std::vector<std::shared_ptr<Player>> {
  std::shared_lock lock(this->players_mutex_);
  std::vector<std::shared_ptr<Player>> ret;
  for (auto [player_id, player] : this->players_) {
    ret.push_back(player);
  }
  return ret;
}

auto WorldManager::GetAOIManager() const -> const AOIManager& {
  return this->aoi_;
}
