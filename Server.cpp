#include "znet/Server.h"

#include "api/Move.h"
#include "api/WorldChat.h"
#include "core/WorldManager.h"

void OnConnectionAdd(IConnection& connection) {
  auto player = std::make_shared<Player>(connection);
  WorldManager::Instance().AddPlayer(player);
  connection.SetProperty("PlayerId", std::make_shared<size_t>(player->GetId()));

  player->SyncPlayerId();
  player->BroadCastStartPosition();
  player->SyncSurrounding();

  LOG(INFO) << "=====> Player pidId = " << player->GetId() << " arrived ====";
}

void OnConnectionDel(IConnection& connection) {
  auto player_id_ret = connection.GetProperty("PlayerId");
  if (player_id_ret == nullptr) {
    LOG(ERROR) << "GetProperty \"PlayerId\" error";
    return;
  }
  auto player_id = *std::static_pointer_cast<size_t>(player_id_ret);
  auto player = WorldManager::Instance().GetPlayer(player_id);
  player->LostConnection();
  LOG(INFO) << "=====> Player pidId = " << player->GetId() << " left ====";
}

int main() {
  auto server = NewServer();

  server->SetOnConnectionStart(OnConnectionAdd);
  server->SetOnConnectionStop(OnConnectionDel);

  server->AddRouter(2, std::make_shared<WorldChatAPI>());
  server->AddRouter(3, std::make_shared<Move>());

  server->Serve();
}