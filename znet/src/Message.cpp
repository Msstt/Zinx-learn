#include "znet/Message.h"

auto Message::GetId() const -> uint32_t { return this->id_; }

auto Message::GetDataLen() const -> uint32_t { return this->data_.size(); }

auto Message::GetData() -> std::vector<uint8_t> & { return this->data_; }

auto Message::GetData() const -> const std::vector<uint8_t> & {
  return this->data_;
}

void Message::SetId(const uint32_t &id) { this->id_ = id; }

void Message::SetDataLen(const uint32_t &data_len) {
  this->data_.resize(data_len);
}