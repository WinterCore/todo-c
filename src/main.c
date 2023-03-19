#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <ncurses.h>
#include <unistd.h>
#include "aids.h"
#include "todo.h"
#include "render.h"
#include "hector.h"

WINDOW *create_new_win(int x, int y, int w, int h) {
    WINDOW *win = newwin(h, w, y, x);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

int main() {
    char *directory = "./";
    char *file = ".data";
    char *path = malloc(sizeof(char) * 100);
    strcat(path, directory);
    strcat(path, file);
    FILE *fd = open_file_rw(path);

    struct Hector *todos = read_todos(fd);
    struct Hector *pending_todos = filter_todos(todos, Todo);
    struct Hector *completed_todos = filter_todos(todos, Completed);
    hector_destroy(todos);

    initscr();
    start_color();
    noecho();
    use_default_colors();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    int halfw = COLS / 2;

    WINDOW *left_win = create_new_win(0, 0, halfw, LINES - 1);
    WINDOW *right_win = create_new_win(halfw, 0, halfw, LINES - 1);
    WINDOW *controls_win = create_new_win(0, LINES - 1, COLS, 1);
    keypad(left_win, true);
    keypad(right_win, true);

    render_controls(controls_win);

    struct GlobalState state = {
        .pending_todos = pending_todos,
        .completed_todos = completed_todos,
        .exit = false,
        .active_pane = 0,
        .should_do_full_render = true,
        .should_write_to_disk = false,
    };

    struct PaneState left_pane_state = {
        .win = left_win,
        .label = "Todo",
        .todos = pending_todos,
        .selected = 0,
        .padding = 1,
    };

    struct PaneState right_pane_state = {
        .win = right_win,
        .label = "Completed",
        .todos = completed_todos,
        .selected = 0,
        .padding = 1,
    };
    
    while (! state.exit) {
        bool left_active = state.active_pane % 2 == 0;
        bool right_active = state.active_pane % 2 == 1;

        if (state.should_do_full_render) {
            render_pane(&left_pane_state, left_active);
            render_pane(&right_pane_state, right_active);
            state.should_do_full_render = false;
        }

        if (state.should_write_to_disk) {
            fseek(fd, 0, SEEK_SET);
            ftruncate(fileno(fd), 0);
            write_todos(fd, state.pending_todos);
            write_todos(fd, state.completed_todos);
            fflush(fd);
        }

        if (left_active) {
            pane_loop(&state, &left_pane_state);
        } else if (right_active) {
            pane_loop(&state, &right_pane_state);
        }
    }

    endwin();

    return 0;
}

