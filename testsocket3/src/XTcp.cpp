#include "XTcp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#define socklen_t int
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define closesocket close
#endif
XTcp::XTcp()
{

    static bool first = true;
    if (first)
    {
        // 只有第一次的时候初始化网络库
        first = false;
        #ifdef WIN32
        WSADATA ws;
        WSAStartup(MAKEWORD(2,2), &ws);
        #endif

    }
    

}

XTcp::~XTcp()
{
}

int XTcp::CreateSocket(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock<0)
    {
        /* code */
        printf("Socket Error\n");
        return -1;
    }
    printf("Socket Created,id:%d\n", sock);
    return sock;
}
bool XTcp::Bind(unsigned short port){
    if (sock<=0){
        CreateSocket();
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // htons是把主机字节序转换为网络字节序
    addr.sin_addr.s_addr = htonl(0); // inet_addr把点分十进制转换为网络字节序的整数
    
    // 绑定端口
    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) != 0)
    {
        printf("Bind Error\n");
        return false;
    }
    printf("Socket Bind to Port %d\n", port);
    
    // 监听端口
    // 第二个参数是最大连接数，超过这个数的连接将被拒绝
    // accept函数从完成连接的队列中取出一个连接，然后返回一个新的socket用于通信
    if (listen(sock, 5) != 0)
    {
        printf("Listen Error\n");
        return false;
    }
    return true;
}

XTcp XTcp::Accept(){
    XTcp client;
    sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_sock = accept(sock, (sockaddr*)&client_addr, &len);
    if (client_sock <= 0)
    {
        return client;
    }
    printf("Accept a connection from client %d \n", client_sock);
    client.ip = inet_ntoa(client_addr.sin_addr);
    client.port = ntohs(client_addr.sin_port);
    client.sock = client_sock;
    printf("Client IP: %s, Port: %d\n", client.ip.c_str(), client.port);
    return client;
}
void XTcp::Close(){
    if (sock>0)
    {
        closesocket(sock);
        sock = 0;
    }
    if (sock<=0)
    {
        /* code */
        return;
    }
    
}

int XTcp::Recv(char *buf, int bufsize){
    return recv(sock, buf, bufsize, 0);
}
int XTcp::Send(const char *buf, int sendsize){
    int sentSize = 0;
    while (sentSize != sendsize)
    {
        /* code */
        int len = send(sock,buf+sentSize, sendsize-sentSize, 0);
        if(len<=0)break;
        sentSize += len;
    } 
    
    return sentSize;
}

bool XTcp::Connect(const string &ip, unsigned short port,int timeoutms){

    if (sock <= 0) {
        CreateSocket();
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    SetBlock(false);
    fd_set set;
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
        FD_ZERO(&set);
        FD_SET(sock, &set);
        timeval tm;
        tm.tv_sec = 0 ;
        tm.tv_usec = timeoutms /1000;
        if(select(sock + 1, NULL, &set, NULL, &tm)<=0){
            printf("Connect Timeout\n");
            printf("IP: %s, Port: %d\n failed:%s\n", ip.c_str(), port, strerror(errno));
            return false;
        }
    }
    SetBlock(true);
    printf("Connect success\n");
    printf("IP: %s, Port: %d\n", ip.c_str(), port);
    this->ip = ip;
    this->port = port;
    return true;
}
bool XTcp::SetBlock(bool isBlock){
    if (sock <= 0) {
        return false;
    }
    // socket的非阻塞模式设置
    #ifdef WIN32
    unsigned long ul = isBlock ? 0 : 1;
    ioctlsocket(sock, FIONBIO, &ul);
    #else
    // 获取当前socket的标志
    int flags = fcntl(sock, F_GETFL, 0); 
    if(flags<0) return false; // 获取失败，直接返回false

    if (isBlock) {
        flags &= ~O_NONBLOCK;
    } else {
        flags |= O_NONBLOCK;
    }
    // 设置socket的标志
    if(fcntl(sock, F_SETFL, flags)!=0) return false;
    #endif
    return true;
}