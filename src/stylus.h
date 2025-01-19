#ifndef STYLUS_H
#define STYLUS_H

#include "common.h"

#include "screen.h"

typedef struct GTStylus GTStylus;
typedef union GTStylusEventData GTStylusEventData;
typedef struct GTStylusEvent GTStylusEvent;

typedef enum GTStylusEventType {
    GT_STYLUS_EVENT_X,
    GT_STYLUS_EVENT_Y,
    GT_STYLUS_EVENT_PRESSURE,
} GTStylusEventType;

union GTStylusEventData {
    int x;
    int y;
    int pressure;
};

struct GTStylusEvent {
    GTStylusEventType event;
    GTStylusEventData data;
};

void GTStylusCreate(GTStylus *stylus);
void GTStylusBind(GTStylus *stylus, GTPoint client, GTScreenList *screens, int screen);
void GTStylusSendEvent(GTStylus *stylus, GTStylusEvent event);
void GTStylusUpdate(GTStylus *stylus, int x, int y, int pressure);

GTPoint GTStylusMapCoord(GTStylus *stylus, GTPoint pt);

#if defined(__gnu_linux__)

struct GTStylus {
    GTPoint display;
    GTPoint client;
    GTRect window;
    int fd;
};

#endif

#endif
