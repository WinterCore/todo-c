#include <ncurses.h>
#include "todo.h"
#include "hector.h"

#ifndef RENDER
#define RENDER

struct GlobalState {
    struct Hector *pending_todos;
    struct Hector *completed_todos;
    bool exit;
    unsigned int active_pane;
    bool should_do_full_render;
    bool should_write_to_disk;
};

struct PaneState {
    WINDOW *win;
    struct Hector *todos;
    size_t selected;
    unsigned int padding;
    char *label;
    unsigned int scroll_pos;
};

void render_label(WINDOW *win, char *label);
void render_controls(WINDOW *win);
void render_todos(
    struct PaneState *state,
    bool active
);
void render_pane(struct PaneState *state, bool active);

void pane_loop(
    struct GlobalState *state,
    struct PaneState *pane_state
);
char *prompt_text_dialog(const int max_len);

void draw_backlight(WINDOW *win, int x, int y, int w, int h);

#endif
