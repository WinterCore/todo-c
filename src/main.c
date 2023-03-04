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

    size_t selected = 0;
    struct Hector *todos = read_todos(fd);

    initscr();
    start_color();
    noecho();
    use_default_colors();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    int halfw = COLS / 2;

    WINDOW *left = create_new_win(0, 0, halfw, LINES);
    keypad(left, true);

    bool exit = false;
    
    while (! exit) {
        render(left, todos, selected);

        int c = wgetch(left);
        switch (c) {
            case 'q':
            case 'Q': {
                exit = true;
                break;
            };

            case KEY_UP:
            case 'k':
            case 'K': {
                if (selected > 0) {
                    selected -= 1;
                }

                break;
            };

            case KEY_DOWN:
            case 'j':
            case 'J': {
                if (selected < todos->length - 1) {
                    selected += 1;
                }

                break;
            };

            case 'd':
            case 'D': {
                if (todos->length == 0) {
                    break;
                }

                delete_todo(hector_get(todos, selected));
                hector_splice(todos, selected, 1);

                if (selected >= hector_size(todos)) {
                    selected -= 1;
                }

                break;
            };

            case 'n':
            case 'N': {
                char *text = prompt_text_dialog(MAX_TODO_TEXT_LEN);
                struct Todo *todo = create_todo(text, Todo);
                hector_push(todos, &todo);
                break;
            };
            case ' ':
            case '\n':
            case KEY_ENTER: {
                toggle_todo_status(hector_get(todos, selected));

                break;
            };
        }
    }

    endwin();

    return 0;
}

