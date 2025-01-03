#pragma once
#include <string>

#include "utils/Prelude.h"
#include "utils/param_deliver.h"

class GlobalObject {
 public:
  static auto Instance() -> GlobalObject &;

 private:
  GlobalObject();
  GlobalObject(const GlobalObject &) = delete;
  auto operator=(const GlobalObject &) -> GlobalObject & = delete;

  void LoadConfig();

 public:
  std::string name_{""};
  std::string host_{"127.0.0.1"};
  uint16_t port_{7777};
  std::string version_{"V0.4"};

  uint32_t max_packet_size_{4096};
  uint32_t max_connection_number_{3};
  uint32_t worker_pool_size_{10};
  uint32_t max_worker_task_length_{1024};
  uint32_t max_data_buffer_size_{1024};
};