#pragma once
#include "utils/prelude.h"

class IMessage {
public:
  virtual ~IMessage() = default;

  virtual auto GetId() const -> uint32_t = 0;
  virtual auto GetDataLen() const -> uint32_t = 0;
  virtual auto GetData() -> std::vector<uint8_t> & = 0;
  virtual auto GetData() const -> const std::vector<uint8_t> & = 0;

  virtual void SetId(const uint32_t &) = 0;
  virtual void SetDataLen(const uint32_t &) = 0;
};