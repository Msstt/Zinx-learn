#include "core/Player.h"

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

  std::string data_stream = data.SerializeAsString();
  // DLOG(INFO) << "              ===> stream: " << data_stream;

  Message msg;
  msg.SetId(msg_id);
  msg.SetDataLen(data_stream.size());
  for (size_t i = 0; i < data_stream.size(); i++) {
    msg.GetData()[i] = static_cast<uint8_t>(data_stream[i]);
  }

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