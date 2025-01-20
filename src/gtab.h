#ifndef GTAB_H
#define GTAB_H

#include "common.h"

#include <math.h>

#include "stylus.h"
#include "screen.h"

#define MODE_CLIENT 1
#define MODE_SERVER 2

#define HELP_MODE_C "  --client   : run as client\n"\
                    "   -c\n"
#define HELP_MODE_S "  --server   : run as server\n"\
                    "   -s\n"
#define HELP_A      "  --addr <a> : the address to connect/bind to\n"\
                    "   -a\n"
#define HELP_P      "  --port <p> : the port to bind/connect to\n"\
                    "   -p\n"
#define HELP_T      "  --test [s] : tests the virtual stylus on screen <s>\n"\
                    "   -t          defaults to the first one it finds\n"
#define HELP_MODE   HELP_MODE_C HELP_MODE_S
#define HELP_ALL    HELP_MODE HELP_A HELP_P HELP_T
#define HELP_USAGE  "Usage: %s [args...]\n" HELP_ALL

typedef struct GTEnv GTEnv;
struct GTEnv {
    GTScreenList screens;
    int port;
    int mode;
    const char *addr;
};

typedef struct GTHandleData GTHandleData;
struct GTHandleData {
    int fd;
    int started;
};

int GTRunClient(GTEnv env);
int GTRunServer(GTEnv env);
void *GTHandleClient(void *data);

int GTParseAddress(const char *addr, struct in6_addr *result);

#endif
