# HTTP Server Project

这是一个基于C++的HTTP服务器项目。

## 项目描述

本项目旨在构建一个高性能的HTTP服务器，支持基本的HTTP请求处理。

## 功能特性

- HTTP/1.1协议支持
- 多线程处理
- 静态文件服务
- 可配置的服务器参数

## 构建说明

```bash
# 使用CMake构建
mkdir build
cd build
cmake ..
make
```

## 运行说明

```bash
./httpserver [port]
```

## 项目结构

```
httpServer/
├── src/           # 源代码目录
├── include/       # 头文件目录
├── test/          # 测试文件目录
├── CMakeLists.txt # CMake配置文件
└── README.md      # 项目说明文档
```