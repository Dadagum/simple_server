#include "utils/simple_server.h"

namespace simple_server {

void errExit(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

}