#pragma once
#include "core/Grid.h"
#include "utils/Prelude.h"

class AOIManager {
 public:
  AOIManager(int, int, size_t, int, int, size_t);

  // 根据 grid_id 获取周围格子的 grid_id
  auto GetSurroundGrids(size_t) const -> std::vector<int>;
  // 根据 (x, y) 获取_周围_格子的 player_ids
  auto GetPlayerIds(double x, double y) const -> std::vector<int>;

  // 根据 grid_id 获取_当前_格子的 player_ids
  auto GetPlayerIds(size_t grid_id) const -> std::vector<int>;
  void AddPlayerId(size_t grid_id, int player_id);
  void AddPlayerId(double x, double y, int player_id);
  void RemovePlayerId(size_t grid_id, int player_id);
  void RemovePlayerId(double x, double y, int player_id);

 private:
  auto GridIdToXY(size_t) const -> std::pair<size_t, size_t>;
  auto XYToGridId(size_t, size_t) const -> size_t;
  auto GetGridId(double x, double y) const -> int;

  int min_x_;
  int max_x_;
  size_t cnt_x_;
  int min_y_;
  int max_y_;
  size_t cnt_y_;
  int width_{};
  int length_{};

  std::vector<std::shared_ptr<Grid>> grids_{};
};