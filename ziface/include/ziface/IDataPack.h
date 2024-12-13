#pragma once
#include "utils/prelude.h"
#include "ziface/IMessage.h"

class IDataPack {
public:
  virtual ~IDataPack() = default;

  virtual auto GetHeadLen() -> uint32_t = 0;
  // 封包
  virtual auto Pack(const IMessage &, std::vector<uint8_t> &) -> bool = 0;
  // 解包
  virtual auto UnPack(const std::vector<uint8_t> &, IMessage &) -> bool = 0;
};