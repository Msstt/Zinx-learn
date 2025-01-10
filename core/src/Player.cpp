#include "core/Player.h"

#include "core/WorldManager.h"

auto PlayerId::Instance() -> PlayerId& {
  static PlayerId instance;
  return instance;
}

auto PlayerId::GetPlayerId() -> size_t {
  std::unique_lock lock(this->mutex_);
  return ++this->player_id_;
}

Player::Player(IConnection& connection) : connection_(connection) {
  this->player_id_ = PlayerId::Instance().GetPlayerId();
  this->x_ = 160 + rand() % 10;
  this->z_ = 160 + rand() % 17;
}

auto Player::GetId() -> size_t { return this->player_id_; }

void Player::SendMsg(uint32_t msg_id, const google::protobuf::Message& data) {
  // LOG(INFO) << "Ready to send ===> id: " << msg_id << " data: ";
  // LOG(INFO) << data.DebugString();

  Message msg;
  msg.SetId(msg_id);
  ProtobufToMessage(data, msg);

  this->connection_.SendMsg(msg);
}

void Player::SyncPlayerId() {
  pb::SyncPlayerId msg;
  msg.set_playerid(this->player_id_);
  this->SendMsg(SIGNIN_MSGID, msg);
}

void Player::BroadCastStartPosition() {
  pb::BroadCast msg;
  msg.set_playerid(this->player_id_);
  msg.set_type(BROADCAST_SIGNIN);
  msg.mutable_position()->set_x(this->x_);
  msg.mutable_position()->set_y(this->y_);
  msg.mutable_position()->set_z(this->z_);
  msg.mutable_position()->set_v(this->v_);

  auto player_ids =
      WorldManager::Instance().GetAOIManager().GetPlayerIds(this->x_, this->z_);
  for (auto player_id : player_ids) {
    auto player = WorldManager::Instance().GetPlayer(player_id);
    if (player == nullptr) {
      continue;
    }
    player->SendMsg(BROADCAST_MSGID, msg);
  }
}

void Player::Talk(const std::string& content) {
  pb::BroadCast msg;
  msg.set_playerid(this->player_id_);
  msg.set_type(BROADCAST_TALK);
  *msg.mutable_content() = content;

  auto players = WorldManager::Instance().GetAllPlayers();
  for (const auto& player : players) {
    player->SendMsg(BROADCAST_MSGID, msg);
  }
}

auto Player::GetPositionMsg() -> pb::Player {
  pb::Player msg;
  msg.set_playerid(this->player_id_);
  msg.mutable_position()->set_x(this->x_);
  msg.mutable_position()->set_y(this->y_);
  msg.mutable_position()->set_z(this->z_);
  msg.mutable_position()->set_v(this->v_);
  return msg;
}

void Player::SyncSurrounding() {
  pb::SyncPlayers msg;
  auto player_ids =
      WorldManager::Instance().GetAOIManager().GetPlayerIds(this->x_, this->z_);
  for (auto player_id : player_ids) {
    auto position_msg =
        WorldManager::Instance().GetPlayer(player_id)->GetPositionMsg();
    *msg.add_players() = position_msg;
  }
  this->SendMsg(SYNC_MSGID, msg);
}

void Player::Move(double x, double y, double z, double v) {
  auto old_grid_id =
      WorldManager::Instance().GetAOIManager().GetGridId(this->x_, this->z_);
  auto new_grid_id = WorldManager::Instance().GetAOIManager().GetGridId(x, z);
  if (old_grid_id != new_grid_id) {
    this->ChangeGrid(old_grid_id, new_grid_id);
  }
  this->x_ = x;
  this->y_ = y;
  this->z_ = z;
  this->v_ = v;

  pb::BroadCast msg;
  msg.set_playerid(this->player_id_);
  msg.set_type(BROADCAST_MOVE);
  msg.mutable_position()->set_x(this->x_);
  msg.mutable_position()->set_y(this->y_);
  msg.mutable_position()->set_z(this->z_);
  msg.mutable_position()->set_v(this->v_);

  auto players = WorldManager::Instance().GetAllPlayers();
  for (const auto& player : players) {
    player->SendMsg(BROADCAST_MSGID, msg);
  }
}

void Player::LostConnection() {
  WorldManager::Instance().RemovePlayer(this->player_id_);

  pb::SyncPlayerId msg;
  msg.set_playerid(this->player_id_);

  auto player_ids =
      WorldManager::Instance().GetAOIManager().GetPlayerIds(this->x_, this->z_);
  for (auto player_id : player_ids) {
    auto player = WorldManager::Instance().GetPlayer(player_id);
    if (player == nullptr) {
      continue;
    }
    player->SendMsg(SIGNOUT_MSGID, msg);
  }
}

bool IsExist(const std::vector<size_t>& vec, size_t num) {
  auto p = std::lower_bound(vec.begin(), vec.end(), num);
  if (p == vec.end()) {
    return false;
  }
  return *p == num;
}

std::vector<size_t> GetGridId(const std::vector<size_t>& old_surround,
                              const std::vector<size_t>& new_surround) {
  std::vector<size_t> ret;
  for (auto grid_id : old_surround) {
    if (IsExist(new_surround, grid_id)) {
      continue;
    }
    ret.push_back(grid_id);
  }
  return ret;
}

void Player::ChangeGrid(size_t old_grid_id, size_t new_grid_id) {
  WorldManager::Instance().GetAOIManager().RemovePlayerId(old_grid_id,
                                                          this->player_id_);

  auto old_surround =
      WorldManager::Instance().GetAOIManager().GetSurroundGrids(old_grid_id);
  auto new_surround =
      WorldManager::Instance().GetAOIManager().GetSurroundGrids(new_grid_id);
  auto del_grid_ids = GetGridId(old_surround, new_surround);
  for (const auto& grid_id : del_grid_ids) {
    auto player_ids =
        WorldManager::Instance().GetAOIManager().GetPlayerIds(grid_id);
    for (const auto& player_id : player_ids) {
      // 自己下线其他玩家
      pb::SyncPlayerId msg;
      msg.set_playerid(player_id);
      this->SendMsg(SIGNOUT_MSGID, msg);

      // 其他玩家下线自己
      msg.set_playerid(this->player_id_);
      auto player = WorldManager::Instance().GetPlayer(player_id);
      if (player == nullptr) {
        continue;
      }
      player->SendMsg(SIGNOUT_MSGID, msg);

      LOG(INFO) << "Player_" << this->player_id_ << " left player_" << player_id
                << "'s scene";
    }
  }

  auto add_grid_ids = GetGridId(new_surround, old_surround);
  for (const auto& grid_id : add_grid_ids) {
    auto player_ids =
        WorldManager::Instance().GetAOIManager().GetPlayerIds(grid_id);
    for (const auto& player_id : player_ids) {
      auto player = WorldManager::Instance().GetPlayer(player_id);
      if (player == nullptr) {
        continue;
      }
      // 自己上线其他玩家
      pb::BroadCast msg;
      msg.set_type(BROADCAST_SIGNIN);
      msg.set_playerid(player_id);
      msg.mutable_position()->set_x(player->x_);
      msg.mutable_position()->set_y(player->y_);
      msg.mutable_position()->set_z(player->z_);
      msg.mutable_position()->set_v(player->v_);
      this->SendMsg(BROADCAST_MSGID, msg);

      // 其他玩家下线自己
      msg.set_playerid(this->player_id_);
      msg.mutable_position()->set_x(this->x_);
      msg.mutable_position()->set_y(this->y_);
      msg.mutable_position()->set_z(this->z_);
      msg.mutable_position()->set_v(this->v_);
      player->SendMsg(BROADCAST_MSGID, msg);

      LOG(INFO) << "Player_" << this->player_id_ << " received player_"
                << player_id << "'s scene";
    }
  }

  WorldManager::Instance().GetAOIManager().AddPlayerId(new_grid_id,
                                                            this->player_id_);
}
