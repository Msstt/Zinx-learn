# Zinx-learn

依赖：
- gtest
- glog
- boost-asio
- nlohmann-json

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