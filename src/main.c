#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <ncurses.h>
#include "aids.h"
#include "todo.h"
#include "render.h"
#include <unistd.h>

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
    char *data = read_file(fd);

    size_t num_todos = 0;
    size_t selected = 0;
    struct Todo **todos = parse(&num_todos, data);

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
        render(left, todos, num_todos, selected);

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
                if (selected < num_todos - 1) {
                    selected += 1;
                }

                break;
            };

            case 'd':
            case 'D': {
                if (num_todos == 0) {
                    break;
                }

                delete_todo(todos[selected]);

                // Check if the removed todo is the last one
                if (selected < num_todos - 1) {
                    memmove(
                        todos + selected,
                        todos + selected + 1,
                        (num_todos - (selected + 1)) * sizeof(**todos)
                    );
                }

                num_todos -= 1;

                if (selected >= num_todos) {
                    selected -= 1;
                }

                break;
            };

            case 'n':
            case 'N': {
                char *text = prompt_text_dialog();

                free(text);
                break;
            };
            case ' ':
            case '\n':
            case KEY_ENTER: {
                toggle_todo_status(todos[selected]);

                break;
            };
        }
    }

    endwin();

    return 0;
}

