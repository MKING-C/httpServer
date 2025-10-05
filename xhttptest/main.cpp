#include "XTcp.h"
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <regex>
using namespace std;
class HttpThread{
public:
    void Main(){
        printf("HttpThread::Main called, this=%p\n", this);
        char buf[10000] = {0};

        int recvlen = client_sock.Recv(buf,sizeof(buf)-1);
        if (recvlen <= 0){
            Close();
            return;
        }
        printf("%s",buf);
        printf("----------rev---------------\n");
        
        // 指定规则 
        // string src = buf;
        // string pattern = "^([A-Z]+) (.+) HTTP/1 ";
        // regex r(pattern);
        // smatch mas;
        // regex_search(src, mas, r);
        // if (mas.size()==0)
        // {
        //     /* code */
        //     printf("%s failed!\n",pattern.c_str());
        //     Close();
        //     return;
        // }
        
        // string type = mas[1];
        // string path = mas[2];
        // printf("%s,%s", type,path);
        // if (type != "GET")
        // {
        //     /* code */
        //     return Close();
        // }
        // string filename;
        // if (path == "/") 
        // {
        //     /* code */
        //     filename = "/index.html";
        // }
        // string filepath = "www";
        // filepath += filename;
        // FILE *fp = fopen(filename.c_str(),"rb");
        // if (fp == NULL)
        // {
        //     /* code */
        //     Close();
        //     return;
        // }
        // // 获取文件大小
        // fseek(fp,0,SEEK_END);
        // int filesize = ftell(fp); 
        // fseek(fp,0,0);
        // printf("file size is %d\n",filesize);

        //回应get请求
        //消息头
        string rmsg = "";
        rmsg = "HTTP/1.1 200 OK \r\n";
        rmsg += "Server:XHttp\r\n";
        rmsg += "Content-Type: text/html\r\n";
        rmsg += "Content-Length: ";
        char bsize[128] = {0};
        // sprintf(bsize,"%d",filesize);
        // rmsg += bsize ;
        rmsg += "10\r\n";
        rmsg += "\r\n";
        //消息内容
        rmsg += "0123456789";
        //发送消息
        int sentSize = client_sock.Send(rmsg.c_str(),rmsg.size());
        printf("sentSize = %d\n", sentSize );
        printf("------------send----------%s\n",rmsg.c_str());
        
        // 发送正文
        // for(;;){
        //     int len = fread(buf,1,sizeof(buf),fp);
        //     if (len <= 0)
        //     {
        //         /* code */
        //         break;
        //     }
        //     int re = client_sock.Send(buf,len);
        //     if(re <= 0) break;
        // }
        Close();
        // // 接收数据
        // char buf[1024] = {0} ;
        
        // // buf是接收数据的缓冲区，sizeof(buf)是缓冲区的大小，0是标志位，
        // // 表示接收任意长度的数据。
        // for (;;)
        // {
        //     int recv_len = client_sock.Recv(buf, sizeof(buf)-1);
        //     if (recv_len <= 0){
        //         printf("Connection closed\n");
        //         break;
        //     }

        //     buf[recv_len] = '\0'; // 添加字符串结束符

        //     if(strstr(buf, "exit") != NULL){
        //         printf("Exit command received, closing connection.\n");
        //         client_sock.Send("Goodbye\n", 8);
        //         break;
        //     }

        //     int send_len = client_sock.Send("ok\n", 3);
        //     if (send_len <= 0){}
        //     printf("Received data: %s\n", buf);
        // }
        // client_sock.Close();
        // delete this;
    }
    void Close(){
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
        HttpThread *th = new HttpThread();
        th->client_sock = client;
        std::thread sth(&HttpThread::Main, th);
        sth.detach();
    }
    server.Close();
    getchar();
    return 0;
}