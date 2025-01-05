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
  pb::SyncPlayerId data;
  data.set_playerid(this->player_id_);
  DLOG(INFO) << "??" << data.DebugString();
  this->SendMsg(1, data);
}

void Player::BroadCastStartPosition() {
  pb::BroadCast data;
  data.set_playerid(this->player_id_);
  data.set_type(2);
  data.mutable_position()->set_x(this->x_);
  data.mutable_position()->set_y(this->y_);
  data.mutable_position()->set_z(this->z_);
  data.mutable_position()->set_v(this->v_);
  this->SendMsg(200, data);
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