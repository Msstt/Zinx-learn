#include "core/AOI.h"

AOIManager::AOIManager(int min_x, int max_x, size_t cnt_x, int min_y, int max_y,
                       size_t cnt_y)
    : min_x_(min_x),
      max_x_(max_x),
      cnt_x_(cnt_x),
      min_y_(min_y),
      max_y_(max_y),
      cnt_y_(cnt_y) {
  this->width_ = (max_x - min_x) / cnt_x;
  this->length_ = (max_y - min_y) / cnt_y;

  for (size_t i = 0; i < cnt_x; i++) {
    for (size_t j = 0; j < cnt_y; j++) {
      this->grids_.push_back(std::make_shared<Grid>(
          i * cnt_y + j, min_x + i * this->width_,
          min_x + (i + 1) * this->width_, min_y + j * this->length_,
          min_y + (j + 1) * this->length_));
    }
  }
}

auto AOIManager::GridIdToXY(size_t grid_id) const -> std::pair<size_t, size_t> {
  return std::make_pair(grid_id / this->cnt_y_, grid_id % this->cnt_y_);
}

auto AOIManager::XYToGridId(size_t x_id, size_t y_id) const -> size_t {
  return x_id * this->cnt_y_ + y_id;
}

auto AOIManager::GetSurroundGrids(size_t grid_id) const -> std::vector<int> {
  auto exist = [&](size_t x_id, size_t y_id) {
    return x_id < this->cnt_x_ && y_id < this->cnt_y_;
  };

  std::vector<int> ret;
  auto [x_id, y_id] = this->GridIdToXY(grid_id);
  for (int nx = -1; nx <= 1; nx++) {
    for (int ny = -1; ny <= 1; ny++) {
      if (exist(x_id + nx, y_id + ny)) {
        ret.push_back(this->XYToGridId(x_id + nx, y_id + ny));
      }
    }
  }
  return ret;
}

auto AOIManager::GetGridId(double x, double y) const -> int {
  if (x < this->min_x_ || x > this->max_x_ || y < this->min_y_ ||
      y > this->max_y_) {
    return -1;
  }
  int x_id = (x - this->min_x_) / this->width_;
  int y_id = (y - this->min_y_) / this->length_;
  return x_id * this->cnt_y_ + y_id;
}

auto AOIManager::GetPlayerIds(double x, double y) const -> std::vector<int> {
  auto grid_id = this->GetGridId(x, y);
  if (grid_id == -1) {
    return {};
  }
  auto surround_grid_ids = this->GetSurroundGrids(grid_id);
  std::vector<int> ret;
  for (const auto& grid_id : surround_grid_ids) {
    auto player_ids = this->grids_[grid_id]->GetPlayerIDs();
    for (const auto& player_id : player_ids) {
      ret.push_back(player_id);
    }
  }
  return ret;
}

auto AOIManager::GetPlayerIds(size_t grid_id) const -> std::vector<int> {
  if (grid_id > this->grids_.size()) {
    return {};
  }
  return this->grids_[grid_id]->GetPlayerIDs();
}

void AOIManager::AddPlayerId(size_t grid_id, int player_id) {
  if (grid_id > this->grids_.size()) {
    return;
  }
  this->grids_[grid_id]->Add(player_id);
}

void AOIManager::AddPlayerId(double x, double y, int player_id) {
  auto grid_id = this->GetGridId(x, y);
  if (grid_id == -1) {
    return;
  }
  this->grids_[grid_id]->Add(player_id);
}

void AOIManager::RemovePlayerId(size_t grid_id, int player_id) {
  if (grid_id > this->grids_.size()) {
    return;
  }
  this->grids_[grid_id]->Remove(player_id);
}

void AOIManager::RemovePlayerId(double x, double y, int player_id) {
  auto grid_id = this->GetGridId(x, y);
  if (grid_id == -1) {
    return;
  }
  this->grids_[grid_id]->Remove(player_id);
}