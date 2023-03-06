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

    /*
    char *buffer = malloc(sizeof(char) * 15);
    strcpy(buffer, "hello");
    buffer[5] = '\n';
    strcpy(buffer + 6, "world");
    fwrite(buffer, 1, 10, fd);

    FILE *fd = open_file_rw(path);
    */

    struct Hector *todos = read_todos(fd);
    struct Hector *pending_todos = filter_todos(todos, Todo);
    struct Hector *completed_todos = filter_todos(todos, Completed);
    hector_destroy(todos);
    ftruncate(fileno(fd), 0);
    write_todos(fd, pending_todos);

    initscr();
    start_color();
    noecho();
    use_default_colors();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    int halfw = COLS / 2;

    WINDOW *left = create_new_win(0, 0, halfw, LINES);
    WINDOW *right = create_new_win(halfw, 0, halfw, LINES);
    keypad(left, true);
    keypad(right, true);

    unsigned int active_pane = 0;
    size_t selected_left = 0;
    size_t selected_right = 0;

    struct State state = {
        .pending_todos = pending_todos,
        .completed_todos = completed_todos,
        .exit = false,
        .pane = active_pane,
        .should_do_full_render = true,
        .should_write_to_disk = false
    };
    
    while (! state.exit) {
        if (state.should_do_full_render) {
            render(left, pending_todos, selected_left);
            render(right, completed_todos, selected_right);
            state.should_do_full_render = false;
        }

        if (state.should_write_to_disk) {
            ftruncate(fileno(fd), 0);
            fseek(fd, 0, SEEK_SET);
            write_todos(fd, state.pending_todos);
            write_todos(fd, state.completed_todos);
        }

        if (state.pane % 2 == 0) {
            pane_loop(&state, left, pending_todos, &selected_left);
        } else if (state.pane % 2 == 1) {
            pane_loop(&state, right, completed_todos, &selected_right);
        }
    }

    endwin();

    return 0;
}

