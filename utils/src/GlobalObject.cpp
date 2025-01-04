#include "utils/GlobalObject.h"

#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

void GlobalObject::LoadConfig(const std::string& conf_name) {
  try {
    std::ifstream config_file(PROJECT_PATH + std::string("/conf/") + conf_name +
                              ".json");
    if (!config_file.is_open()) {
      LOG(FATAL) << "GlobalObject open \"./conf/zinx.json\" failed";
    }
    nlohmann::json config = nlohmann::json::parse(config_file);

    this->name_ = config["Name"];
    this->host_ = config["Host"];
    this->port_ = config["Post"];
    this->max_connection_number_ = config["MaxConnectionNumber"];
    this->worker_pool_size_ = config["WorkerPoolSize"];
    this->max_worker_task_length_ = config["MaxWorkerTaskLength"];
    this->max_data_buffer_size_ = config["MaxDataBufferSize"];
  } catch (...) {
    LOG(FATAL) << "GlobalObject parse \"./conf/zinx.json\" failed";
  }
}

GlobalObject::GlobalObject() { this->LoadConfig(); }

auto GlobalObject::Instance() -> GlobalObject& {
  static GlobalObject instance;
  return instance;
}