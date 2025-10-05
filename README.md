# HTTP Server Project

这是一个基于C++的HTTP服务器项目集合，包含多个版本的HTTP服务器实现和测试用例。

## 项目描述

本项目包含从基础TCP通信到完整HTTP服务器的一系列实现，展示了HTTP服务器开发的演进过程。

## 项目版本演进

### 基础版本
- **testsocket/** - 基础TCP服务器测试
- **testsocket2/** - 改进的TCP服务器
- **testsocket3/** - 增强版TCP服务器

### HTTP服务器版本
- **xhttptest/** - HTTP服务器基础测试
- **xhttp2/** - 第二版HTTP服务器
- **xhttp3/** - 第三版HTTP服务器
- **xhttp4/** - 第四版HTTP服务器（支持PHP）
- **xhttp5/** - 第五版HTTP服务器
- **xhttpfinal/** - 最终版完整HTTP服务器

### 客户端测试
- **tcpclient/** - TCP客户端测试程序

## 功能特性

- ✅ HTTP/1.1协议支持
- ✅ 多线程并发处理
- ✅ 静态文件服务
- ✅ 动态内容支持（PHP）
- ✅ 可配置服务器参数
- ✅ 完整的HTTP请求/响应处理
- ✅ MIME类型自动识别
- ✅ 目录列表显示
- ✅ 错误页面处理

## 构建说明

每个子目录都包含独立的makefile，可以直接编译：

```bash
# 进入具体版本目录
cd xhttpfinal

# 使用make编译
make

# 运行服务器
./tcpserver/tcpserver [端口号]
```

## 运行说明

### 启动HTTP服务器
```bash
# 默认端口8080
./tcpserver/tcpserver

# 指定端口
./tcpserver/tcpserver 8080
```

### 访问测试
启动服务器后，可以通过浏览器访问：
- `http://localhost:8080` - 首页
- `http://localhost:8080/test.html` - 测试页面
- `http://localhost:8080/1.jpg` - 图片文件

## 项目结构

```
httpServer/
├── tcpclient/          # TCP客户端测试程序
├── testsocket/         # 基础TCP服务器
├── testsocket2/        # 改进版TCP服务器
├── testsocket3/        # 增强版TCP服务器
├── xhttptest/          # HTTP服务器基础测试
├── xhttp2/             # 第二版HTTP服务器
├── xhttp3/             # 第三版HTTP服务器
├── xhttp4/             # 第四版HTTP服务器（支持PHP）
├── xhttp5/             # 第五版HTTP服务器
├── xhttpfinal/         # 最终版完整HTTP服务器
│   ├── include/        # 头文件目录
│   ├── src/            # 源代码目录
│   ├── tcpserver/      # 服务器可执行文件
│   ├── www/            # 网页文件目录
│   └── makefile        # 构建文件
└── README.md           # 项目说明文档
```

## 技术栈

- **语言**: C++
- **网络**: Socket编程
- **协议**: HTTP/1.1
- **构建**: Makefile
- **平台**: Windows/Linux跨平台

## 开发环境

- 支持GCC/Clang编译器
- 需要C++11或更高版本支持
- 在Windows和Linux系统上测试通过