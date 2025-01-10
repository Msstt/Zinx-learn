#include "core/Grid.h"

void Grid::Add(size_t player_id) {
  std::unique_lock lock(this->mutex_);
  this->player_ids_.insert(player_id);
}

void Grid::Remove(size_t player_id) {
  std::unique_lock lock(this->mutex_);
  this->player_ids_.erase(player_id);
}

auto Grid::GetPlayerIDs() const -> std::vector<size_t> {
  std::shared_lock lock(this->mutex_);
  std::vector<size_t> ret;
  for (const auto& player_id : this->player_ids_) {
    ret.push_back(player_id);
  }
  return ret;
}