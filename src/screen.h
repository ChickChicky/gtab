#ifndef SCREENS_H
#define SCREENS_H

#include "common.h"

typedef struct GTScreenInfo GTScreenInfo;
struct GTScreenInfo {
    Window window;
    GTRect location;
};

typedef struct GTScreenList GTScreenList;
struct GTScreenList {
    GTPoint size;
    GTScreenInfo screens[MAX_SCREENS];
    size_t count;
};

typedef struct GTScreenEnv GTScreenEnv;

GTScreenList GTScreenFetch(void);

#endif
