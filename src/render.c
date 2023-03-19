#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include "hector.h"
#include "render.h"
#include "aids.h"

void render_pane(
    struct PaneState *state,
    bool active
) {
    werase(state->win);

    render_label(state->win, state->label);
    render_todos(state, active);

    wrefresh(state->win);
}

void render_controls(WINDOW *win) {

    werase(win);
    // Render Controls
    wmove(win, LINES - 1, 1);
    waddstr(win, "q: Quit - k: Up - j: Down - n: New todo - space: Switch pane - Enter: Toggle todo");
    wrefresh(win);
}

void render_label(WINDOW *win, char *label) {
    // Render label
    wmove(win, 0, 1);
    waddstr(win, label);
}

void render_todos(
    struct PaneState *ps,
    bool active
) {
    box(ps->win, 0, 0);
    int cols = 0;
    int rows = 0;
    getmaxyx(ps->win, rows, cols);
    int viewport_lines = rows - (ps->padding * 2);
    size_t todos_size = hector_size(ps->todos);

    if (ps->selected >= viewport_lines + ps->scroll_pos) {
        ps->scroll_pos += 1;
    }

    if (ps->selected < ps->scroll_pos) {
        ps->scroll_pos -= 1;
    }

    if (ps->scroll_pos > todos_size - viewport_lines) {
        ps->scroll_pos = todos_size - viewport_lines;
    }

    int selected_y = (ps->selected - ps->scroll_pos) + ps->padding;

    size_t i = 0;
    size_t j = ps->scroll_pos;
    size_t l = MIN(todos_size, rows - (ps->padding * 2));

    // Render todos
    while (i < l) {
        if (active && ps->selected == j) {
            wattron(ps->win, COLOR_PAIR(1));
            draw_backlight(
                ps->win,
                ps->padding,
                selected_y,
                cols - ps->padding * 2,
                1
            );
        }
        
        int y = i + ps->padding;

        wmove(ps->win, y, ps->padding);
        waddstr(ps->win, "[ ");
        struct Todo *todo = hector_get(ps->todos, j);
        if (todo->status == Completed) {
            wmove(ps->win, y, 1 + ps->padding);
            waddstr(ps->win, "x");
        }

        wmove(ps->win, y, 2 + ps->padding);
        waddstr(ps->win, "]: ");

        wmove(ps->win, y, 5 + ps->padding);
        waddstr(ps->win, todo->data);

        if (active && ps->selected == j) {
            wattroff(ps->win, COLOR_PAIR(1));
        }


        i += 1;
        j += 1;
    }

    wmove(ps->win, selected_y, 1 + ps->padding);
}

char *prompt_text_dialog(const int max_len) {
    char *buffer = malloc(sizeof(char) * max_len + 1);
    buffer[0] = '\0';
    int w = 60;
    int h = 8;
    int x = (COLS - w) / 2;
    int y = (LINES - h) / 2;
    int padding_y = 1;
    int padding_x = 5;
    int input_w = w - padding_x * 2;
    WINDOW *win = newwin(h, w, y, x);
    bool cancel_selected = false;

    int i = 0;
    while (true) {
        werase(win);
        box(win, 0, 0);

        // Render Controls
        wmove(win, h - 1, 1);
        waddstr(win, " Enter: Create todo | Tab: Cycle between controls ");

        if (cancel_selected) {
            wattron(win, COLOR_PAIR(2));
        }
        wmove(win, h - 3, 3);
        waddstr(win, "Cancel");
        if (cancel_selected) {
            wattroff(win, COLOR_PAIR(2));
        }

        if (! cancel_selected) {
            wattron(win, COLOR_PAIR(2));
        }
        draw_backlight(
            win,
            padding_x,
            padding_y + 1,
            input_w,
            1
        );

        wmove(win, padding_y + 1, padding_x);
        waddstr(win, (buffer + MAX(0, i - input_w)));

        if (! cancel_selected) {
            wattroff(win, COLOR_PAIR(2));
        }

        wrefresh(win);

        int c = wgetch(win);

        switch (c) {
            case KEY_STAB:
            case '\t': {
                cancel_selected = ! cancel_selected;
                break;
            };
            case KEY_ENTER:
            case '\n': {
                if (cancel_selected) {
                    return NULL;
                }

                return buffer;
            };

            case KEY_BACKSPACE:
            case '\b':
            case 127: {
                if (cancel_selected) {
                    break;
                }

                if (i > 0) {
                    i -= 1;
                    buffer[i] = '\0';
                }
                break;
            };

            default: {
                if (cancel_selected) {
                    break;
                }

                if (! isalnum(c) && c < -128 && 127 < c) {
                    break;
                }

                if (i >= 200) {
                    break;
                }

                buffer[i] = c;
                buffer[i + 1] = '\0';
                i += 1;
                break;
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

void pane_loop(
    struct GlobalState *state,
    struct PaneState *ps
) {
    render_pane(ps, true);

    int c = wgetch(ps->win);

    switch (c) {
        case 'q':
        case 'Q': {
            state->exit = true;
            break;
        };

        case KEY_UP:
        case 'k':
        case 'K': {
            if (ps->selected > 0) {
                ps->selected -= 1;
            }

            break;
        };

        case KEY_DOWN:
        case 'j':
        case 'J': {
            if (ps->todos->length != 0
                && ps->selected < ps->todos->length - 1) {
                ps->selected += 1;
            }

            break;
        };

        case 'd':
        case 'D': {
            if (ps->todos->length == 0) {
                break;
            }

            delete_todo(hector_get(ps->todos, ps->selected));
            hector_splice(ps->todos, ps->selected, 1);

            size_t todos_num = hector_size(ps->todos);

            if (ps->selected >= todos_num && todos_num > 0) {
                ps->selected -= 1;
            }

            state->should_write_to_disk = true;

            break;
        };

        case 'n':
        case 'N': {
            char *text = prompt_text_dialog(MAX_TODO_TEXT_LEN);
            state->should_do_full_render = true;

            if (text == NULL) {
                break;
            }

            struct Todo *todo = create_todo(text, Todo);
            hector_push(state->pending_todos, todo);
            state->should_write_to_disk = true;

            break;
        };

        case '\n':
        case KEY_ENTER: {
            if (ps->todos->length == 0) {
                break;
            }

            struct Todo *todo = hector_get(ps->todos, ps->selected);
            toggle_todo_status(todo);

            if (todo->status == Todo) {
                hector_splice(ps->todos, ps->selected, 1);
                hector_push(state->pending_todos, todo);
            } else if (todo->status == Completed) {
                hector_splice(ps->todos, ps->selected, 1);
                hector_push(state->completed_todos, todo);
            }

            size_t todos_num = hector_size(ps->todos);

            if (ps->selected >= todos_num && todos_num > 0) {
                ps->selected -= 1;
            }

            state->should_do_full_render = true;
            state->should_write_to_disk = true;

            break;
        };

        case ' ': {
            state->active_pane ^= 1;
            state->should_do_full_render = true;

            break;
        };
    }
}
