#include "XTcp.h"
#include <thread>
#include <stdlib.h>
#include <string.h>
using namespace std;
class TcpThread{
public:
    void Main(){

        // 接收数据
        char buf[1024] = {0} ;
        
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
        /* code */
        port = atoi(argv[1]);
    }

    XTcp server;
    // server.CreateSocket();
    server.Bind(port);
    for (;;)
    {
        /* code */
        XTcp client = server.Accept();
        TcpThread *tcpThread = new TcpThread();
        tcpThread->client_sock = client;
        std::thread sth(&TcpThread::Main, tcpThread);
        sth.detach();
    }
    server.Close();
    getchar();
    return 0;
}