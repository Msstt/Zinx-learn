#!/bin/bash

protoc --cpp_out=. ./msg/msg.proto

mv ./msg/msg.pb.cc ./src/msg.pb.cpp
mv ./msg/msg.pb.h ./include/protobuf/msg.pb.h

sed -i '' 's/#include "msg\/msg.pb.h"/#include "protobuf\/msg.pb.h"/' ./src/msg.pb.cpp