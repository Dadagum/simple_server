#ifndef SIMPLE_SERVER_TCP_SERVER_H
#define SIMPLE_SERVER_TCP_SERVER_H

#include "utils/simple_server.h"

namespace simple_server {

/* 开启 TCP server */
int startTcpServer(int port, char* ip = nullptr, int backlog = 5);

}
#endif