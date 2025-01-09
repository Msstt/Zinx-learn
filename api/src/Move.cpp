#include "api/Move.h"

void Move::PreHandle(IRequest &) {}

void Move::Handle(IRequest &request) {
  auto player_id_ret = request.GetConnection().GetProperty("PlayerId");
  if (player_id_ret == nullptr) {
    LOG(ERROR) << "GetProperty \"PlayerId\" error";
    return;
  }
  auto player_id = *std::static_pointer_cast<size_t>(player_id_ret);
  auto player = WorldManager::Instance().GetPlayer(player_id);
  if (player == nullptr) {
    LOG(ERROR) << "GetPlayer \"" << player_id << "\" error";
    return;
  }
  pb::Position msg;
  MessageToProtobuf(request.GetMsg(), msg);
  LOG(INFO) << "Player_" << player_id << " update position: " << msg.x() << " "
            << msg.y() << " " << msg.z() << " " << msg.v();
  player->Move(msg.x(), msg.y(), msg.z(), msg.v());
}

void Move::PostHandle(IRequest &) {}