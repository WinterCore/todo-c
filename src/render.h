#include <ncurses.h>
#include "todo.h"
#include "hector.h"

#ifndef RENDER
#define RENDER

struct State {
    struct Hector *pending_todos;
    struct Hector *completed_todos;
    bool exit;
    unsigned int pane;
    bool should_do_full_render;
    bool should_write_to_disk;
};

void render(WINDOW *win, struct Hector *todos, size_t selected);
void render_pane(
    struct State *state,
    WINDOW *win,
    struct Hector *todos,
    size_t *selected
);
char *prompt_text_dialog(const int max_len);

void draw_backlight(WINDOW *win, int x, int y, int w, int h);

#endif
