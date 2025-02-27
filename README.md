# Zinx-learn

## 依赖

- gtest
- glog
- boost-asio
- nlohmann-json
- protobuf

- 客户端模块：https://github.com/Msstt/ZINX-Client

## 部署方式

## 配置文件

- `Name`：服务器名称
- `Host`：监听的 `IP` 地址
- `Post`：监听的端口号
- `MaxConnectionNumber`：最大连接数
- `WorkerPoolSize`：处理请求的线程数，`0` 表示为每个请求创建一个线程
- `MaxWorkerTaskLength`：请求等待队列的最大长度
- `MaxDataBufferSize`：请求内容的最大字节数
- `AOIMinX`，`AOIMaxX`，`AOIMinY`，`AOIMaxY`：`AOI` 地图的边界
- `AOICountX`，`AOICountY`：`AOI` 地图的划分块数
- `BornMinX`，`BornMaxX`，`BornMinY`，`BornMaxY`：玩家出生位置的随机区域

## 更新日志

V0: 搭建 Zinx 框架
V1: 实现 MMO 服务器

V0.1
- 回显服务器

V0.2
- 添加 Connection，封装业务逻辑

V0.3
- 添加 Request，Router，基础路由模块

V0.4
- 添加 GlobalObject，支持读入配置文件

V0.5
- 添加 Message，DataPack，封装消息，实现封包解包，解决 TCP 粘包问题

V0.6
- 添加 MsgHandle，实现多路由模式

V0.7
- 添加 writer 线程，实现读写分离
- 添加错误码
- 添加简易线程池
- TODO:
  - 线程池删除结束线程

V0.8
- 添加 worker 线程池，task 任务队列，缓冲 handle msg 时创建销毁线程的代价
- 添加模板同步队列

V0.9
- 添加 ConnManager，实现连接管理，拒绝超出最大链接数的请求
- 实现注册链接[启动/停止]自定义函数
- 添加单进程集成测试

V0.10
- 实现链接属性设置

V1.1
- 添加 AOIManager，实现 2D 地图的划分、九宫格信息更新与查询

V1.2
- 添加 protobuf 消息类，实现与 Unity 客户端的通信
- 添加 Player 类管理用户
- 实现用户登录功能

V1.3
- 添加 WorldManager 类，管理 AOIManager 和所有的 Player
- 添加 WorldChatAPI 类，实现世界聊天

V1.4
- 实现上线位置的信息同步功能，客户端能够看到其他玩家

V1.5
- 实现移动功能，完成移动后位置的信息同步

V1.6
- 实现下线功能

V1.7
- 实现玩家跨越 AOI 格子时的位置同步

V1.8
- 重构线程管理
  - 将特殊线程改由各个类自行管理
  - 不使用多任务机制时，Connection 类使用 ThreadPool 管理线程(始终新建线程)

V1.8.1
- 支持配置玩家出生位置
- 修复位置信息同步广播对象不是相邻格子玩家的 bug

V1.8.2
- 支持命令行指定配置文件
- 完善文档：部署方式
- 上传 client