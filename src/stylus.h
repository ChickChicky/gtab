#ifndef STYLUS_H
#define STYLUS_H

#include "common.h"

#include "screen.h"

typedef struct GTStylus GTStylus;

void GTStylusCreate(GTStylus *stylus);
void GTStylusBind(GTStylus *stylus, GTPoint client, GTScreenList *screens, int screen);
void GTStylusUpdate(GTStylus *stylus, int x, int y, int pressure);

GTPoint GTStylusMapCoord(GTStylus *stylus, GTPoint pt);

#if defined(__gnu_linux__)

struct GTStylus {
    GTPoint display;
    GTPoint client;
    GTRect window;
    int fd;
};

#else

struct GTStylus {
    GTPoint display;
    GTPoint client;
    GTRect window;
};

#warning "Platform not supported for stylus"

#endif

#endif
