#pragma once
#include "ziface/IMessage.h"

class Message : public IMessage {
public:
  auto GetId() const -> uint32_t override;
  auto GetDataLen() const -> uint32_t override;
  auto GetData() -> std::vector<uint8_t> & override;
  auto GetData() const -> const std::vector<uint8_t> & override;

  void SetId(const uint32_t &) override;
  void SetDataLen(const uint32_t &) override;

private:
  uint32_t id_{0};
  std::vector<uint8_t> data_{};
};