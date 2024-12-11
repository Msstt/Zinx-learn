#include "znet/Request.h"

auto Request::GetConnection() -> IConnection & { return this->connection_; }

auto Request::GetData() -> std::vector<uint8_t> & { return this->data_; }