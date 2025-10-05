#pragma once
#include <string>
using namespace std;

class XHttpResponse
{
private:
    /* data */
    int filesize = 0;
    FILE *fp = NULL;
public:
    bool SetRequest(string request);
    string GetHead();
    int Read(char *buf,int bufsize);
    XHttpResponse(/* args */);
    ~XHttpResponse();
};

