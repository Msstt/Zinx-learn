#include "google/protobuf/text_format.h"

#include "znet/Server.h"
#include "core/Player.h"

void OnConnectionAdd(IConnection& connection) {
  auto player = Player(connection);

  player.SyncPlayerId();
  player.BroadCastStartPosition();

  LOG(INFO) << "=====> Player pidId = " << player.GetId() << " arrived ====";
}

int main() {
  auto server = NewServer();
  server->SetOnConnectionStart(OnConnectionAdd);

  server->Serve();
}