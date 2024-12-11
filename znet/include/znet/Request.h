#pragma once
#include "ziface/IRequest.h"
#include "znet/Connection.h"

class Request : public IRequest {
public:
  Request(IConnection &connection, std::vector<uint8_t> data)
      : connection_(connection), data_(std::move(data)) {}

  auto GetConnection() -> IConnection & override;
  auto GetData() -> std::vector<uint8_t> & override;

private:
  IConnection &connection_;
  std::vector<uint8_t> data_;
};