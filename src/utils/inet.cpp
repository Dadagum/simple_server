#include "utils/inet.h"

namespace simple_server {

sockaddr_in getInetAddr(sa_family_t family, int port, char* ip) {
    sockaddr_in addr;
    addr.sin_family = family;
    addr.sin_port = htons(port);
    if (ip) {
        if (0 == inet_aton(ip, &addr.sin_addr)) {
            errExit("%s:%d - invalid ip addr, use INADDR_ANY instead!\n", __FILE__, __LINE__);
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
    } else addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return addr;
}

}