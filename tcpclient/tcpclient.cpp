# include "XTcp.h"
int main(){
    XTcp client;
    client.SetBlock(false);
    client.Connect("172.19.1.106", 8080 , 3000);
    
    client.Send("Hello", 5);
    char buf[1024] = {0};
    client.Recv(buf, 1024);
    printf("Recv from server: %s\n", buf);
    getchar();
    client.Close();

    return 0;
} 