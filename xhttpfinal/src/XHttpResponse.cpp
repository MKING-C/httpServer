#include "XHttpResponse.h"
#include <string>
#include <regex>
#include <cstdio>
XHttpResponse::XHttpResponse(/* args */)
{
}

XHttpResponse::~XHttpResponse()
{
    if (fp != NULL){
        fclose(fp);
        fp = NULL;
    }
}
bool XHttpResponse::SetRequest(string request){
    //指定规则 
    string src = request;
    // 规则的写法是一个正则表达式，用来匹配HTTP请求的头信息
    string pattern = "^([A-Z]+) /([a-zA-Z0-9]*([.][a-zA-Z]*)?)[?]?(.*) HTTP/1";
    regex r(pattern);
    smatch mas; // 正则表达式匹配结果
    regex_search(src, mas, r); // 执行匹配操作
    // 匹配失败
    if (mas.size()==0)
    {
        /* code */
        printf("%s failed!\n",pattern.c_str());
        return false;
    }
    
    //匹配成功
    string type = mas[1];
    string path = "/";
    path += mas[2];
    string filetype = mas[3];
    string query = mas[4];

    if (type != "GET")
    {
        /* code */
        printf("Not get!!\n");
        return false;
    }

    // 处理路径
    string filename = path;
    if (path == "/") 
    {
        /* code */
        // 请求页面
        filename = "/index.html";
    }
    string filepath = "www";
    filepath += filename;

    if (filetype == "php")
    {
        /* code */
        string cmd = "php-cgi ";
        cmd += filepath;
        cmd += " ";

        for(int i = 0; i < (int)query.size(); i++){
            if (query[i]=='&'){
                query[i] = ' ';
            }
        }
        cmd += query;
        
        cmd += " > ";
        filepath += ".html";
        cmd += filepath;
        printf("%s\n",cmd.c_str());
        // 将cmd字符串转换为C风格字符串
        // 调用操作系统shell来执行这个命令
        system(cmd.c_str());

        // php-cgi在执行PHP脚本时，会自动输出HTTP响应头信息，
        // 包括Content-Type: text/html; charset=UTF-8等
    }
    
    // 关闭之前可能打开的文件句柄
    if (fp != NULL){
        fclose(fp);
        fp = NULL;
    }
    // 打开php脚本生成的文件
    fp = fopen(filepath.c_str(),"rb");
    // 打开失败或者没有文件
    if (fp == NULL)
    {
        /* code */
        printf("open file failed");
        return false;
    }

    //打开成功
    // 获取文件大小
    fseek(fp,0,SEEK_END);
    filesize = ftell(fp); 
    fseek(fp,0,0);
    printf("file size is %d\n",filesize);
    if (filetype=="php")
    {
        /* code */
        //去除Content-Type: text/html; charset=UTF-8等，多余的消息
        char c = 0;
        // 统计正文删除了多少多余的消息
        int headsize = 0;
        while(fread(&c,1,1,fp)>0){
            headsize++;
            // "\r\n\r\n"是HTTP消息头结束的标志;
            if (c=='\r')
            {
                fseek(fp,3,SEEK_CUR);
                headsize+=3;
                break;
                /* code */
            }
            
        }
        // 修改发送文件的大小
        filesize = filesize-headsize;
    }
    // 将文件发送给客户端
    //回应get请求
    
    //消息内容
    // rmsg += "0123456789";
    //发送消息
    // int sentSize = client_sock.Send(rmsg.c_str(),rmsg.size());
    // printf("sentSize = %d\n", sentSize );
    // printf("------------send----------%s\n",rmsg.c_str());
    
    // 发送正文
    // for(;;){
    //     int len = fread(buf,1,sizeof(buf),fp);
    //     if (len <= 0)
    //     {
    //         /* code */
    //         break;
    //     }
    //     // Send做了特殊处理的，保证能发完所有消息
    //     int re = client_sock.Send(buf,len);
    //     if(re <= 0) break;
    // }
    return true;

}
string XHttpResponse::GetHead(){
    //消息头
    string rmsg = "";
    rmsg = "HTTP/1.1 200 OK \r\n";
    rmsg += "Server:XHttp\r\n";
    rmsg += "Content-Type: text/html\r\n";
    rmsg += "Content-Length: ";
    char bsize[128] = {0};
    sprintf(bsize,"%d",filesize);
    rmsg += bsize ;
    // rmsg += "10\r\n";
    rmsg += "\r\n\r\n"; //消息头结束标志
    return rmsg;
}
int XHttpResponse::Read(char *buf,int bufsize){
    if (fp == NULL) return -1;
    size_t r = fread(buf, 1, (size_t)bufsize, fp);
    return (int)r;
}