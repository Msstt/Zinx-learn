#pragma once
#include "ziface/IDataPack.h"

/*
  datalen + msgid + data
*/

class DataPack : public IDataPack {
public:
  auto GetHeadLen() -> uint32_t override;
  auto Pack(const IMessage &, std::vector<uint8_t> &) -> bool override;
  auto UnPack(const std::vector<uint8_t> &, IMessage &) -> bool override;
};