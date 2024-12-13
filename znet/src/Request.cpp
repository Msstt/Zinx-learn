#include "znet/Request.h"

auto Request::GetConnection() -> IConnection & { return this->connection_; }

auto Request::GetMsg() const -> const IMessage & { return this->msg_; }