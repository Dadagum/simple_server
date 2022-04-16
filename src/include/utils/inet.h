#ifndef SIMPLE_SERVER_INET_H
#define SIMPLE_SERVER_INET_H

#include "utils/simple_server.h"

namespace simple_server {

sockaddr_in getInetAddr(sa_family_t family, int port, char* ip = nullptr);

}

#endif