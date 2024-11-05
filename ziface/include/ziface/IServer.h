#pragma once

class IServer {
public:
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual void Serve() = 0;
};