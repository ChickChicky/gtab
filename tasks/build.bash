#!/usr/bin/env bash

set -xe

C_SOURCES="src/gtab.c src/screen.c src/stylus.c src/threads.c"
C_OUTPUT="gtab"
C_LIBS="-lX11 -lm -lpthread"
C_INCLUDES="-Isrc"
C_DEFINES="-DGT_USE_X11"

gcc -Wall -Wextra --pedantic $C_DEFINES $C_INCLUDES $C_SOURCES -o $C_OUTPUT $C_LIBS
