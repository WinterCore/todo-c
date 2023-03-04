#include <ncurses.h>
#include "todo.h"
#include "hector.h"

#ifndef RENDER
#define RENDER

void render(WINDOW *win, struct Hector *todos, size_t selected);
char *prompt_text_dialog(const int max_len);

void draw_backlight(WINDOW *win, int x, int y, int w, int h);

#endif
