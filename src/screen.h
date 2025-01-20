#ifndef SCREENS_H
#define SCREENS_H

#include "common.h"

#if GT_GENV == GT_GENV_X11

typedef struct GTScreenInfo GTScreenInfo;
struct GTScreenInfo {
    Window window;
    GTRect location;
};

#else

typedef struct GTScreenInfo GTScreenInfo;
struct GTScreenInfo {
    GTRect location;
};

#warning "Platform is not supported for screen"

#endif

typedef struct GTScreenList GTScreenList;
struct GTScreenList {
    GTPoint size;
    GTScreenInfo screens[MAX_SCREENS];
    size_t count;
};

GTScreenList GTScreenFetch(void);

#endif
