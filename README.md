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

V0.8
- 添加 worker 线程池，task 任务队列，缓冲 handle msg 时创建销毁线程的代价
- 添加模板同步队列

V0.9
- 添加 ConnManager，实现连接管理，拒绝超出最大链接数的请求
- 实现注册链接[启动/停止]自定义函数
- 添加单进程集成测试

V0.10
- 实现链接属性设置