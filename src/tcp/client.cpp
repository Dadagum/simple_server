#include "tcp/client.h"
#include "utils/inet.h"

namespace simple_server {

int connectServer(int port, char* ip) { 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = getInetAddr(AF_INET, port, ip);
    int res = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) 
        errExit("%s:%d - connect() fails!\n", __FILE__, __LINE__);
    return sockfd;
}

}