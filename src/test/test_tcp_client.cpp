#include "tcp/client.h"

void tcpClient() {
    int sockfd = simple_server::connectServer(8081, "127.0.0.1");
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    int cnt;
    while ((cnt = read(0, buf, BUF_SIZE-1)) > 0) {
        write(sockfd, buf, cnt);
        cnt = read(sockfd, buf, BUF_SIZE-1);
        buf[cnt] = '\0';
        write(1, buf, cnt+1);
    }
    close(sockfd);
}

int main() {
    tcpClient();
}