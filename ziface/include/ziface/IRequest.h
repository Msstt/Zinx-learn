#pragma once
#include "ziface/IConnection.h"

class IRequest {
public:
  virtual ~IRequest() = default;
  virtual auto GetConnection() -> IConnection & = 0;
  virtual auto GetData() -> std::vector<uint8_t> & = 0;
};