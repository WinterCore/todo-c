#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include "hector.h"
#include "render.h"
#include "aids.h"

void render(
    WINDOW *win,
    struct Hector *todos,
    size_t selected
) {
    size_t i = 0;

    werase(win);

    box(win, 0, 0);
    int cols = 0;
    int rows = 0;
    int padding = 1;
    int selected_y = selected + padding;

    getmaxyx(win, rows, cols);

    // Render todos
    while (i < todos->length) {
        if (selected == i) {
            wattron(win, COLOR_PAIR(1));
            draw_backlight(win, padding, selected_y, cols - padding * 2, 1);
        }
        
        int y = i + padding;

        wmove(win, y, padding);
        waddstr(win, "[ ");
        struct Todo *todo = hector_get(todos, i);
        if (todo->status == Completed) {
            wmove(win, y, 1 + padding);
            waddstr(win, "x");
        }

        wmove(win, y, 2 + padding);
        waddstr(win, "]: ");

        wmove(win, y, 5 + padding);
        waddstr(win, todo->data);

        if (selected == i) {
            wattroff(win, COLOR_PAIR(1));
        }


        i += 1;
    }

    // Render Controls
    wmove(win, LINES - padding, padding);
    waddstr(win, " q: Quit  k: Up  j: Down  n: New ");

    wmove(win, selected_y, 1 + padding);
    wrefresh(win);
}

char *prompt_text_dialog(const int max_len) {
    char *buffer = malloc(sizeof(char) * max_len + 1);
    int w = 60;
    int h = 6;
    int x = (COLS - w) / 2;
    int y = (LINES - h) / 2;
    int padding_y = 1;
    int padding_x = 5;
    int input_w = w - padding_x * 2;
    WINDOW *win = newwin(h, w, y, x);

    int i = 0;
    while (true) {
        werase(win);
        box(win, 0, 0);

        // Render Controls
        wmove(win, h - 1, 1);
        waddstr(win, " Enter: Create todo  Tab: Cycle between controls ");


        wattron(win, COLOR_PAIR(2));
        draw_backlight(
            win,
            padding_x,
            padding_y + 1,
            input_w,
            1
        );

        wmove(win, padding_y + 1, padding_x);
        waddstr(win, (buffer + MAX(0, i - input_w)));

        wattroff(win, COLOR_PAIR(2));

        wrefresh(win);

        int c = wgetch(win);

        switch (c) {
            case KEY_ENTER:
            case '\n': {
                return buffer;
            };

            case KEY_BACKSPACE:
            case '\b':
            case 127: {
                if (i > 0) {
                    i -= 1;
                    buffer[i] = '\0';
                }
                break;
            };

            default: {
                if (c < -128 && 127 < c) {
                    break;
                }

                if (i >= 200) {
                }

                buffer[i] = c;
                buffer[i + 1] = '\0';
                i += 1;
            };
        }
    }

    return buffer;
}

void draw_backlight(WINDOW *win, int x, int y, int w, int h) {
    for (int row = 0; row < h; row += 1) {

        for (int col = 0; col < w; col += 1) {
            wmove(win, y + row, x + col);
            waddch(win, ' ');
        }
    }
}

void render_pane(
    struct State *state,
    WINDOW *win,
    struct Hector *todos,
    size_t *selected
) {
    render(win, todos, *selected);

    int c = wgetch(win);

    switch (c) {
        case 'q':
        case 'Q': {
            state->exit = true;
            break;
        };

        case KEY_UP:
        case 'k':
        case 'K': {
            if (*selected > 0) {
                *selected -= 1;
            }

            break;
        };

        case KEY_DOWN:
        case 'j':
        case 'J': {
            if (*selected < todos->length - 1) {
                *selected += 1;
            }

            break;
        };

        case 'd':
        case 'D': {
            if (todos->length == 0) {
                break;
            }

            delete_todo(hector_get(todos, *selected));
            hector_splice(todos, *selected, 1);

            if (*selected >= hector_size(todos)) {
                *selected -= 1;
            }

            state->should_write_to_disk = true;

            break;
        };

        case 'n':
        case 'N': {
            char *text = prompt_text_dialog(MAX_TODO_TEXT_LEN);
            struct Todo *todo = create_todo(text, Todo);
            hector_push(todos, todo);
            state->should_do_full_render = true;
            state->should_write_to_disk = true;

            break;
        };

        case '\n':
        case KEY_ENTER: {
            struct Todo *todo = hector_get(todos, *selected);
            toggle_todo_status(todo);

            if (todo->status == Todo) {
                hector_splice(todos, *selected, 1);
                hector_push(state->pending_todos, todo);
            } else if (todo->status == Completed) {
                hector_splice(todos, *selected, 1);
                hector_push(state->completed_todos, todo);
            }

            if (*selected >= hector_size(todos)) {
                *selected -= 1;
            }

            state->should_do_full_render = true;
            state->should_write_to_disk = true;

            break;
        };

        case ' ': {
            state->pane += 1;

            break;
        };
    }
}
