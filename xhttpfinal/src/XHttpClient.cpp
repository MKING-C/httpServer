#include "XHttpClient.h"
#include <thread>
XHttpClient::XHttpClient(){

}
XHttpClient::~XHttpClient(){

}
bool XHttpClient::Start(XTcp client){
    // 服务端accpet后传递的client
    this->client = client;
    // 创建线程并指定主程序
    thread sth(&XHttpClient::Main,this);
    sth.detach();
    return true;
}
void XHttpClient::Main(){
    //接收数据
    char buf[10240]{0};
    for(;;){
        int len = client.Recv(buf,sizeof(buf)-1);
        if(len<=0){
            break;
        }
        buf[len] = '\0'; //每次发送的结尾处加\0
        if (!res.SetRequest(buf))
        {
            break;
        }
        //发消息头
        string head = res.GetHead();
        if (client.Send(head.c_str(),head.size()) <= 0)
        {
            // 发送失败退出
            /* code */
            break;
        }
        //发送内容
        int size = sizeof(buf);
        bool error = false;
        for (;;)
        {
            int len = res.Read(buf, size);
            if (len < 0)
            {
                // 没有数据读取了就退出
                error = true;
                break;
                /* code */
            }
            if(len == 0) break;
            if(client.Send(buf,len) <= 0){
                error = true; 
                break;
                /* code */
            }
        }
        
    }
    client.Close();
    delete this;
}
