#include "znet/Message.h"

Message::Message(uint32_t id, const std::string &data) : id_(id) {
  this->data_.resize(data.size());
  for (size_t i = 0; i < data.size(); i++) {
    this->data_[i] = static_cast<uint8_t>(data[i]);
  }
}

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

auto Message::ToString() const -> std::string {
  std::string ret;
  for (const auto &c : this->data_) {
    ret += static_cast<char>(c);
  }
  return ret;
}

void MessageToProtobuf(const IMessage &msg, google::protobuf::Message &pb_msg) {
  std::string data_stream(msg.GetData().begin(), msg.GetData().end());
  pb_msg.ParseFromString(data_stream);
}

void ProtobufToMessage(const google::protobuf::Message &pb_msg, IMessage &msg) {
  std::string data_stream = pb_msg.SerializeAsString();
  msg.SetDataLen(data_stream.size());
  for (size_t i = 0; i < data_stream.size(); i++) {
    msg.GetData()[i] = static_cast<uint8_t>(data_stream[i]);
  }
}