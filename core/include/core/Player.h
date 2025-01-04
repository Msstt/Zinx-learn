#pragma once
#include "protobuf/msg.pb.h"
#include "znet/Connection.h"
#include "znet/Message.h"

class PlayerId {
 public:
  static auto Instance() -> PlayerId&;

  auto GetPlayerId() -> size_t;

 private:
  PlayerId() = default;
  PlayerId(const PlayerId&) = delete;

  std::mutex mutex_{};
  size_t player_id_{0};
};

class Player {
 public:
  Player(IConnection&);

  auto GetId() -> size_t;

  void SyncPlayerId();

  void BroadCastStartPosition();

 private:
  void SendMsg(uint32_t, const google::protobuf::Message&);

  size_t player_id_;
  IConnection& connection_;

  double x_;
  double y_{0};
  double z_;
  double v_{0};
};