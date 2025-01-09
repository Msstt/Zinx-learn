#pragma once
#include "core/AOI.h"
#include "core/Player.h"

class WorldManager {
 public:
  static auto Instance() -> WorldManager&;

  void AddPlayer(const std::shared_ptr<Player>&);
  void RemovePlayer(const size_t&);
  auto GetPlayer(const size_t&) const -> std::shared_ptr<Player>;
  auto GetAllPlayers() const -> std::vector<std::shared_ptr<Player>>;

  auto GetAOIManager() const -> const AOIManager&;

 private:
  WorldManager()
      : aoi_(GlobalObject::Instance().aoi_min_x_,
             GlobalObject::Instance().aoi_max_x_,
             GlobalObject::Instance().aoi_count_x_,
             GlobalObject::Instance().aoi_min_y_,
             GlobalObject::Instance().aoi_max_y_,
             GlobalObject::Instance().aoi_count_y_) {}
  WorldManager(const WorldManager&) = delete;

  AOIManager aoi_;
  mutable std::shared_mutex players_mutex_{};
  std::map<size_t, std::shared_ptr<Player>> players_{};
};