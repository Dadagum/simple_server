#include <sys/epoll.h>
#include <pthread.h>
#include "tcp/server.h"

/* 为何客户端 ctrl D 和 ctrl C 的效果不同呢？ */
void* worker(void* arg) {
    pthread_detach(pthread_self());
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    int cli_fd = *((int*)arg);
    int cnt = 0;
    while ((cnt = read(cli_fd, buf, BUF_SIZE-1)) > 0) {
        buf[cnt] = '\0';
        printf("server get:\n %s", buf); // 直接打印接收的字节
        write(cli_fd, buf, cnt); // 重新告诉客户端接收的字节
    }
    return NULL;
}

/**
 * @brief tcp server 示例程序
 * 
 */
void simpleTcpEchoServer() {
    int sockfd = simple_server::startTcpServer(8081);
    int cli_fd;
    sockaddr_in cli_addr;
    socklen_t cli_addr_sz = sizeof(cli_addr);
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    while (true) {
        // 设置了 non-blocking 的 server
        while (true) {
            cli_fd = accept(sockfd, (sockaddr*)&cli_addr, &cli_addr_sz);
            if (cli_fd == -1) {
                if (errno == EWOULDBLOCK) {
                    printf("No pending connections; sleeping for one second.\n");
                    sleep(1);
                } else {
                    simple_server::errExit("%s:%d - accept() fails!\n", __FILE__, __LINE__);
                }
            } else break;
        }
        // 业务处理：直接 echo
        int cnt = 0;
        while ((cnt = read(cli_fd, buf, BUF_SIZE-1)) > 0) {
            buf[cnt] = '\0';
            printf("server get: %s", buf); // 直接打印接收的字节
            write(cli_fd, buf, cnt); // 重新告诉客户端接收的字节
        }
        printf("closing connection with fd = %d \n", cli_fd);
        close(cli_fd);
    }
}

void epollEchoServer() {
    setvbuf(stdout, NULL, _IONBF, 0);
    int ret;
    const int MAX_EVENTS = 1024;
    epoll_event evlist[MAX_EVENTS];
    // 开启服务端
    int sockfd = simple_server::startTcpServer(8081);
    // 创建 epoll
    int epfd = epoll_create(MAX_EVENTS);
    // 监控 accept 事件
    epoll_event ev;
    ev.events = EPOLLIN;  
    ev.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) < 0) 
        simple_server::errExit("%s:%d - epoll_ctl() fails!\n", __FILE__, __LINE__);
    // 客户端地址信息
    int cli_fd;
    sockaddr_in cli_addr;
    socklen_t cli_addr_sz = sizeof(cli_addr);
    // 监听客户端请求
    while (true) {
        int ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1); // block until ready
        // 检查就绪事件
        for (int i = 0; i < ready; ++i) {
            if (evlist[i].events & (EPOLLHUP | EPOLLRDHUP)) {
                // 连接关闭
                cli_fd = evlist[i].data.fd;
                printf("server closing connection with fd = %d \n", cli_fd);
                close(cli_fd);
            } else if (evlist[i].events & EPOLLIN) {
                // 读就绪
                if (evlist[i].data.fd == sockfd) {
                    // accept 事件
                    cli_fd = accept(sockfd, (sockaddr*)&cli_addr, &cli_addr_sz);
                    // 设置 nonblocking IO
                    int flag = fcntl(cli_fd, F_GETFL);
                    if (errno == -1) {
                        simple_server::errExit("%s:%d - fcntl() fails!\n", __FILE__, __LINE__);
                    }
                    fcntl(cli_fd, F_SETFL, flag | O_NONBLOCK);
                    // 监听 fd 的可读事件, 采用边缘触发模式
                    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;  
                    ev.data.fd = cli_fd;
                    if ((ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cli_fd, &ev)) < 0) {
                        simple_server::errExit("%s:%d - epoll_ctl() fails with ret = %d!\n", __FILE__, __LINE__, ret);
                    }
                    printf("server accepting fd = %d\n", cli_fd);
                } else {
                    // 客户端读事件
                    // 业务处理：直接 echo
                    cli_fd = evlist[i].data.fd;
                    pthread_t th;
                    pthread_create(&th, NULL, worker, &cli_fd);
                }
            }
        }
    }
}


int main() {
    epollEchoServer();
}