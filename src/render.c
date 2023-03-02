#include "render.h"
#include "todo.h"
#include <ncurses.h>

void render(struct Todo **todos, size_t num_todos, size_t selected) {
    size_t i = 0;

    erase();
    while (i < num_todos) {
        if (selected == i) {
            attron(COLOR_PAIR(1));
        }

        int col = 0;
        while (col < COLS) {
            mvaddch(i, col, ' ');
            col += 1;
        }


        mvaddstr(i, 0, "[ ");
        if (todos[i]->status == Completed) {
            mvaddstr(i, 1, "x");
        }
        mvaddstr(i, 2, "]: ");
        mvaddstr(i, 5, todos[i]->data);

        if (selected == i) {
            attroff(COLOR_PAIR(1));
        }

        i += 1;
    }
    refresh();
}
