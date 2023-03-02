#!/bin/sh

CC="${CXX:-cc}"
CFLAGS="-Wall -Wextra -std=c11 -ggdb"
SRC="src/main.c src/aids.c src/todo.c src/render.c"
PKGS="ncurses"

$CC $CFLAGS `pkg-config --cflags $PKGS` -o todo $SRC `pkg-config --libs $PKGS`
