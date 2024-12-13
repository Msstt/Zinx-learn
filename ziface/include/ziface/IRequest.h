#pragma once
#include "ziface/IConnection.h"
#include "ziface/IMessage.h"

class IRequest {
public:
  virtual ~IRequest() = default;

  virtual auto GetConnection() -> IConnection & = 0;
  virtual auto GetMsg() const -> const IMessage & = 0;
};