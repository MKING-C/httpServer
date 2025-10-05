#pragma once
#include <string>
using namespace std;
class XTcp
{
private:
    /* data */
public:
    int CreateSocket();
    bool Bind(unsigned short port);
    XTcp Accept();
    void Close();
    int Recv(char *buf, int bufsize);
    int Send(const char *buf, int sendsize);
    bool Connect(const string &ip, unsigned short port,int timeoutms=1000);
    bool SetBlock(bool isBlock);
    XTcp();
    virtual ~XTcp();

    int sock = 0;
    unsigned short port = 0;
    string ip = "";
};
