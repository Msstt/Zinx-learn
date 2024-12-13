#include "znet/DataPack.h"

auto DataPack::GetHeadLen() -> uint32_t { return 8; }

auto DataPack::Pack(const IMessage &msg, std::vector<uint8_t> &buf) -> bool {
  buf.resize(8 + msg.GetDataLen());
  LittleEndianWrite(msg.GetDataLen(), buf.data());
  LittleEndianWrite(msg.GetId(), buf.data() + 4);
  std::copy(msg.GetData().begin(), msg.GetData().end(), buf.data() + 8);
  return true;
}

auto DataPack::UnPack(const std::vector<uint8_t> &buf, IMessage &msg) -> bool {
  uint32_t msg_id, data_len;
  LittleEndianRead(buf.data(), data_len);
  LittleEndianRead(buf.data() + 4, msg_id);
  msg.SetId(msg_id);
  msg.SetDataLen(data_len);
  return true;
}