#include "XTcp.h"
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
using namespace std;
class TcpThread{
public:
    void Main(){
        // 
        // 接收数据
        char buf[1024];
        
        // buf是接收数据的缓冲区，sizeof(buf)是缓冲区的大小，0是标志位，
        // 表示接收任意长度的数据。
        for (;;)
        {
            int recv_len = client_sock.Recv(buf, sizeof(buf)-1);
            if (recv_len <= 0){
                printf("Connection closed\n");
                break;
            }

            buf[recv_len] = '\0'; // 添加字符串结束符

            if(strstr(buf, "exit") != NULL){
                printf("Exit command received, closing connection.\n");
                client_sock.Send("Goodbye\n", 8);
                break;
            }

            int send_len = client_sock.Send("ok\n", 3);
            if (send_len <= 0){}
            printf("Received data: %s\n", buf);
        }
        client_sock.Close();
        delete this;
    }
    XTcp client_sock;
};

int main(int argc, char *argv[]){

    unsigned short port = 8080;
    if (argc>1)
    {
        // 判断是否有命令行参数，如果有，则将端口号设置为命令行参数指定的值
        port = atoi(argv[1]);
    }

    XTcp server;
    // server.CreateSocket();
    server.Bind(port);
    // 创建epoll事件池
    int epoll_fd = epoll_create(256);

    // 添加事件到epoll
    epoll_event ev;
    ev.data.fd = server.sock;
    ev.events = EPOLLIN | EPOLLET;
    // 将socket添加到epoll事件池中
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server.sock, &ev);
    epoll_event event[20];
    char buf[1024] = {0};
    const char *msg = "HTTP/1.1 200 OK \r\nContent-Length:1\r\n\r\n";
    int size = strlen(msg);
    server.SetBlock(false);
    for (;;)
    {
        int count = epoll_wait(epoll_fd,event,20,500);
        if(count <= 0) continue;

        for (int i = 0; i < count; i++)
        {
            // 判断事件类型，如果是服务器socket，则处理新连接
            if(event[i].data.fd == server.sock){
                for(;;){
                    XTcp client = server.Accept();
                    if (client.sock <= 0)
                    {
                        break;
                    }
                    ev.data.fd = client.sock;
                    ev.events = EPOLLIN | EPOLLET;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client.sock,&ev);
                }
            }else{
                XTcp client;
                client.sock = event[i].data.fd;
                client.Recv(buf,1024);
                client.Send(msg,size);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client.sock,&ev);
            }
        }
        
        // XTcp client = server.Accept();
        // TcpThread *tcpThread = new TcpThread();
        // tcpThread->client_sock = client;
        // std::thread sth(&TcpThread::Main, tcpThread);
        // sth.detach();
    }
    server.Close();
    getchar();
    return 0;
}