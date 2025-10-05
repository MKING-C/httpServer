#include "XHttpServer.h"
#include "XHttpClient.h"
#include <thread>
using namespace std;
XHttpServer::XHttpServer(/* args */)
{
}

XHttpServer::~XHttpServer()
{
}

bool XHttpServer::Start(unsigned short port){
    isexit = false;
    // server是一个tcp
    server.CreateSocket();
    // 绑定服务端的端口号port
    if (!server.Bind(port))
    {
        return false;
    }
    
    // 通过线程来接收用户的数据
    thread sth(&XHttpServer::Main,this);
    sth.detach(); // 释放主线程的资源
    return true;
}
void XHttpServer::Main(){
    while(!isexit){
        // 接受用户的连接
        XTcp client = server.Accept();
        if(client.sock <= 0) continue;
        // 后面要进行自行销毁，所以要再堆中创建对象
        XHttpClient *th =new XHttpClient();
        // 把处理数据和发送消息交给XHttpClient的一个线程处理
        th->Start(client);
    }
    
}
void XHttpServer::Stop(){
    isexit = true;
 }