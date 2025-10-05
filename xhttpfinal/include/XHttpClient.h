/*
用户交互的类：(不是与服务端进行连接)
    与客户端通信
    对每次的http创建线程对象进行处理
    线程中会接受用户数据，然后调用算法（php），
    然后将结果通过http消息返回给用户
*/
#pragma once
#include "XTcp.h"
#include "XHttpResponse.h"
class XHttpClient
{
private:
    /* data */
public:
    XHttpClient(/* args */);
    ~XHttpClient();
    
    bool Start(XTcp client);
    void Main(); // 入口的线程函数
    XHttpResponse res;
    XTcp client; // 本应该用一个函数进行处理
};
