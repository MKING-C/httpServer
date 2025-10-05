#pragma once
#include "XTcp.h"
class XHttpServer
{
private:
    /* data */
public:

    bool Start(unsigned short port);
    XHttpServer(/* args */);
    ~XHttpServer();
    XTcp server;
    void Main();
    void Stop();

    bool isexit = false; // 控制线程的退出
};
