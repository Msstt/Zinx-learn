#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"
#include "utils/GlobalObject.h"

void GlobalObject::LoadConfig() {
  try {
    std::ifstream config_file(PROJECT_PATH + std::string("/conf/zinx.json"));
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
    this->max_data_buffer_size_= config["MaxDataBufferSize"];
  } catch (...) {
    LOG(FATAL) << "GlobalObject parse \"./conf/zinx.json\" failed";
  }
}

GlobalObject::GlobalObject() { this->LoadConfig(); }

auto GlobalObject::Instance() -> GlobalObject & {
  static GlobalObject instance;
  return instance;
}