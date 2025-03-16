#!/bin/sh
SDL_CFLAGS="$(sdl2-config --cflags)"
SDL_LIBS="$(sdl2-config --libs)"

CFLAGS="-I Musashi/ -O3 -g -DSDL_UI"
CC=cc
SRC=kyacopycat
PRG=kyacopycat
set -e
set -x
cd Musashi && gmake && cd ..

$CC -c -o $PRG.o $CFLAGS $SDL_CFLAGS $SRC.c
$CC -o $PRG $CFLAGS $SDL_CFLAGS $PRG.o Musashi/m68kdasm.o Musashi/m68kcpu.o Musashi/m68kops.o Musashi/softfloat/softfloat.o -lm $SDL_LIBS

