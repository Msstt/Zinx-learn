#pragma once
#include <string>

#include "utils/Prelude.h"
#include "utils/param_deliver.h"

class GlobalObject {
 public:
  static auto Instance() -> GlobalObject &;

  void LoadConfig(const std::string &conf_name = "zinx");

 private:
  GlobalObject();
  GlobalObject(const GlobalObject &) = delete;
  auto operator=(const GlobalObject &) -> GlobalObject & = delete;

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

  int32_t aoi_min_x_{0};
  int32_t aoi_max_x_{0};
  uint32_t aoi_count_x_{0};
  int32_t aoi_min_y_{0};
  int32_t aoi_max_y_{0};
  uint32_t aoi_count_y_{0};

  int32_t born_min_x_{0};
  int32_t born_max_x_{0};
  int32_t born_min_y_{0};
  int32_t born_max_y_{0};
};