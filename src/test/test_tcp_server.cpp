#include "tcp/server.h"

/**
 * @brief tcp server 示例程序
 * 
 */
void simpleTcpEchoServer() {
    int sockfd = simple_server::startTcpServer(8081);
    int cli_fd;
    sockaddr_in cli_name;
    socklen_t cli_name_sz = sizeof(cli_name);
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    while (true) {
        cli_fd = accept(sockfd, (sockaddr*)&cli_name, &cli_name_sz);
        if (cli_fd < 0) {
            simple_server::errExit("%s:%d - accept() fails!\n", __FILE__, __LINE__);
        }
        // 业务处理：直接 echo
        int cnt = 0;
        while ((cnt = read(cli_fd, buf, BUF_SIZE-1)) > 0) {
            buf[cnt] = '\0';
            printf("server get: %s\n", buf); // 直接打印接收的字节
            write(cli_fd, buf, cnt+1); // 重新告诉客户端接收的字节
        }
        close(cli_fd);
    }
}


int main() {
    simpleTcpEchoServer();
}