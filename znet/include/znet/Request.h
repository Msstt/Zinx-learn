#pragma once
#include "ziface/IRequest.h"
#include "znet/Connection.h"
#include "znet/Message.h"

class Request : public IRequest {
public:
  Request(IConnection &connection, Message msg)
      : connection_(connection), msg_(std::move(msg)) {}

  auto GetConnection() -> IConnection & override;
  auto GetMsg()const -> const IMessage & override;

private:
  IConnection &connection_;
  Message msg_;
};