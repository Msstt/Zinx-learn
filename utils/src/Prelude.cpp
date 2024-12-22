#include "utils/Prelude.h"

void LittleEndianWrite(const uint32_t &src, uint8_t *dst) {
  dst[0] = src & 0x000000FF;
  dst[1] = (src >> 8) & 0x000000FF;
  dst[2] = (src >> 16) & 0x000000FF;
  dst[3] = (src >> 24) & 0x000000FF;
}

void LittleEndianRead(const uint8_t *src, uint32_t &dst) {
  dst = 0;
  dst |= static_cast<uint32_t>(src[0]);
  dst |= static_cast<uint32_t>(src[1]) << 8;
  dst |= static_cast<uint32_t>(src[2]) << 16;
  dst |= static_cast<uint32_t>(src[3]) << 24;
}

auto SocketExactRead(ip::tcp::socket &socket, std::vector<uint8_t> &buf)
    -> ErrorKind {
  error_code err;
  uint32_t ptr = 0;
  while (ptr < buf.size()) {
    auto bytes =
        socket.read_some(buffer(buf.data() + ptr, buf.size() - ptr), err);
    if (err) {
      if (err == error::operation_aborted || err == error::eof) {
        return ErrorKind::EOF_;
      }
      LOG(ERROR) << "SocketExactRead failed" << err;
      return ErrorKind::ERROR_;
    }
    ptr += bytes;
  }
  return ErrorKind::OK_;
}

auto SocketExactWrite(const std::vector<uint8_t> &buf, ip::tcp::socket &socket)
    -> ErrorKind {
  error_code err;
  uint32_t ptr = 0;
  while (ptr < buf.size()) {
    auto bytes =
        socket.write_some(buffer(buf.data() + ptr, buf.size() - ptr), err);
    if (err) {
      if (err == error::operation_aborted || err == error::eof) {
        return ErrorKind::EOF_;
      }
      LOG(ERROR) << "SocketExactWrite failed" << err;
      return ErrorKind::ERROR_;
    }
    ptr += bytes;
  }
  return ErrorKind::OK_;
}