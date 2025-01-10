#pragma once
#include <set>
#include <shared_mutex>

#include "utils/Prelude.h"

class Grid {
 public:
  Grid(size_t grid_id, int min_x, int max_x, int min_y, int max_y)
      : grid_id_(grid_id),
        min_x_(min_x),
        max_x_(max_x),
        min_y_(min_y),
        max_y_(max_y) {}

  void Add(size_t player_id);
  void Remove(size_t player_id);
  auto GetPlayerIDs() const -> std::vector<size_t>;

 private:
  size_t grid_id_;
  int min_x_;
  int max_x_;
  int min_y_;
  int max_y_;

  std::set<size_t> player_ids_{};
  mutable std::shared_mutex mutex_{};
};