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
  LOG(INFO) << "Ready to send ===> id: " << msg_id << " data: ";
  LOG(INFO) << data.DebugString();

  Message msg;
  msg.SetId(msg_id);
  ProtobufToMessage(data, msg);

  this->connection_.SendMsg(msg);
}

void Player::SyncPlayerId() {
  pb::SyncPlayerId msg;
  msg.set_playerid(this->player_id_);
  this->SendMsg(1, msg);
}

void Player::BroadCastStartPosition() {
  pb::BroadCast msg;
  msg.set_playerid(this->player_id_);
  msg.set_type(2);
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
    player->SendMsg(200, msg);
  }
}

void Player::Talk(const std::string& content) {
  pb::BroadCast msg;
  msg.set_playerid(this->player_id_);
  msg.set_type(1);
  *msg.mutable_content() = content;

  auto players = WorldManager::Instance().GetAllPlayers();
  for (const auto& player : players) {
    player->SendMsg(200, msg);
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
  this->SendMsg(202, msg);
}
