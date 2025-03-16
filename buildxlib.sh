#!/bin/sh
XLIB_CFLAGS="$(pkg-config --cflags x11)"
XLIB_LIBS="$(pkg-config --libs x11)"

CFLAGS="-I Musashi/ -O3 -g -DXLIB_UI"
CC=cc
SRC=kyacopycat
PRG=kyacopycat_xlib
set -e
set -x
cd Musashi && gmake && cd ..

$CC -c -o $PRG.o $CFLAGS $XLIB_CFLAGS $SRC.c
$CC -o $PRG $CFLAGS $XLIB_CFLAGS $PRG.o Musashi/m68kdasm.o Musashi/m68kcpu.o Musashi/m68kops.o Musashi/softfloat/softfloat.o -lm $XLIB_LIBS
