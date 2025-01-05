#include "znet/Server.h"

#include "api/WorldChat.h"
#include "core/WorldManager.h"

void OnConnectionAdd(IConnection& connection) {
  auto player = std::make_shared<Player>(connection);

  player->SyncPlayerId();
  player->BroadCastStartPosition();

  WorldManager::Instance().AddPlayer(player);
  connection.SetProperty("PlayerId", std::make_shared<size_t>(player->GetId()));

  LOG(INFO) << "=====> Player pidId = " << player->GetId() << " arrived ====";
}

int main() {
  auto server = NewServer();

  server->SetOnConnectionStart(OnConnectionAdd);

  server->AddRouter(2, std::make_shared<WorldChatAPI>());

  server->Serve();
}