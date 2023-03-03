#include <ncurses.h>
#include "todo.h"

#ifndef RENDER
#define RENDER

void render(WINDOW *win, struct Todo **todos, size_t num_todos, size_t selected);
char *prompt_text_dialog();

void draw_backlight(WINDOW *win, int x, int y, int w, int h);

#endif
