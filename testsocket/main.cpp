#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>
#define closesocket close
#endif

#include <thread>
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
            int recv_len = recv(client_sock, buf, sizeof(buf)-1, 0);
            if (recv_len <= 0){
                printf("Connection closed\n");
                break;
            }

            buf[recv_len] = '\0'; // 添加字符串结束符

            if(strstr(buf, "exit") != NULL){
                printf("Exit command received, closing connection.\n");
                send(client_sock, "Goodbye\n", 8, 0);
                break;
            }

            int send_len = send(client_sock, "ok\n", 3, 0);
            if (send_len <= 0){}
            printf("Received data: %s\n", buf);

        }
        closesocket(client_sock);
        delete this;
    }
    int client_sock = 0;
};

int main(int argc, char *argv[]){
    #ifdef WIN32
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
    #endif

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock<0)
    {
        /* code */
        printf("Socket Error\n");
        return -1;
    }
    printf("Socket Created,id:%d\n", sock);

    unsigned short port = 8080;
    if (argc>1)
    {
        /* code */
        port = atoi(argv[1]);
    }
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // htons是把主机字节序转换为网络字节序
    addr.sin_addr.s_addr = htonl(0); // inet_addr把点分十进制转换为网络字节序的整数
    
    // 绑定端口
    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) != 0)
    {
        printf("Bind Error\n");
        return -2;
    }
    printf("Socket Bind to Port %d\n", port);
    
    // 监听端口
    // 第二个参数是最大连接数，超过这个数的连接将被拒绝
    // accept函数从完成连接的队列中取出一个连接，然后返回一个新的socket用于通信
    if (listen(sock, 5) != 0)
    {
        printf("Listen Error\n");
        return -3;
    }

    // int client_sock = accept(sock, NULL, NULL);
    // if (client_sock<0)
    // {
    //     printf("Accept Error\n");
    //     return -4;
    // }
   
    // 保存客户端的地址信息
    for(;;){    
        sockaddr_in client_addr;
        #ifdef WIN32
        int len = sizeof(client_addr);
        #else
        socklen_t len = sizeof(client_addr);
        #endif
        int client_sock = accept(sock, (sockaddr*)&client_addr, &len);
        printf("Accept a connection from client %d \n", client_sock);
        char *ip = inet_ntoa(client_addr.sin_addr);
        unsigned short client_port = ntohs(client_addr.sin_port);
        printf("Client IP: %s, Port: %d\n", ip, client_port);
        
        TcpThread* thread = new TcpThread();
        thread->client_sock = client_sock;
        std::thread sth(&TcpThread::Main, thread);
        sth.detach(); // 创建线程后，主线程继续执行，线程结束后自动回收 

    }
    closesocket(sock);
    getchar();
    return 0;
}