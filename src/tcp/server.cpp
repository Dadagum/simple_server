#include "tcp/server.h"
#include "utils/inet.h"

namespace simple_server {

/**
 * @brief 开启 TCP 服务端
 * 
 * @param port 端口号
 * @param ip ip 地址
 * @param backlog "In the original BSD sockets implementation, the upper limit for backlog was 5"
 * @return int 返回 sockfd
 */
int startTcpServer(int port, char* ip, int backlog) {
    int sockfd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    sockaddr_in addr = getInetAddr(AF_INET, port, ip);
    /* 服务器端使用 SO_REUSEADDR 重用 TIME_WAIT 状态端口 */
    int on = 1;
    if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)  
        errExit("%s:%d - setsockopt() fails!\n", __FILE__, __LINE__);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        errExit("%s:%d - bind() fails!\n", __FILE__, __LINE__);
    if (listen(sockfd, backlog) < 0) 
        errExit("%s:%d - listen() fails!\n", __FILE__, __LINE__);
    else printf("%s:%d - listening at [port=%d]\n", __FILE__, __LINE__, port);
    return sockfd;
}

}