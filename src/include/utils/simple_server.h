#ifndef SIMPLE_SERVER_COMMAND_H
#define SIMPLE_SERVER_COMMAND_H

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

namespace simple_server{

void errExit(const char* format, ...);

}

#endif