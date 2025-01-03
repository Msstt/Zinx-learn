#include "core/Grid.h"

void Grid::Add(int player_id) {
  std::unique_lock lock(this->mutex_);
  this->player_ids_.insert(player_id);
}

void Grid::Remove(int player_id) {
  std::unique_lock lock(this->mutex_);
  this->player_ids_.erase(player_id);
}

auto Grid::GetPlayerIDs() const -> std::vector<int> {
  std::shared_lock lock(this->mutex_);
  std::vector<int> ret;
  for (const auto& player_id : this->player_ids_) {
    ret.push_back(player_id);
  }
  return ret;
}