#ifndef COMMON_H
#define COMMON_H

#include "config.h"

#if !defined(_WIN32) && !defined(__gnu_linux__)
    #error "Only Windows and GNU Linux are supported"
#endif

#define GT_GENV_X11 1
#define GT_GENV_WAYLAND 2
#define GT_GENV_WINDOWS 3

#if defined(_WIN32)
    #define GT_GENV GT_GENV_WINDOWS
#elif defined(GT_USE_X11)
    #define GT_GENV GT_GENV_X11
#elif defined(GT_USE_WAYLAND)
    #define GT_GENV GT_GENV_WAYLAND
#else
    #error "Unsupported platform, try -DGT_USE_X11 or -DGT_USE_WAYLAND"
#endif

#if GT_GENV == GT_GENV_X11
    #include <X11/Xlib.h>
#elif GT_GENV == GT_GENV_WAYLAND
    #include <wayland-client-core.h>
#elif GT_GENV == GT_GENV_WINDOWS
    #include <Windows.h>
#endif

#if defined(__gnu_linux__)
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#endif

#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>

typedef struct GTRect GTRect;
typedef struct GTPoint GTPoint;

struct GTRect {
    int x, y, 
        width, height;
};

struct GTPoint {
    int x, y;
};

#endif
