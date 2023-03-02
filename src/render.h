#include <ncurses.h>
#include "todo.h"

#ifndef RENDER
#define RENDER

void render(struct Todo **todos, size_t num_todos, size_t selected);

#endif
