#include "render.h"
#include "todo.h"
#include <ncurses.h>

void render(
    WINDOW *win,
    struct Todo **todos,
    size_t num_todos,
    size_t selected
) {
    size_t i = 0;

    werase(win);

    box(win, 0, 0);
    int cols = 0;
    int rows = 0;
    int padding = 1;

    getmaxyx(win, rows, cols);

    while (i < num_todos) {
        if (selected == i) {
            wattron(win, COLOR_PAIR(1));
        }
        
        int y = i + padding;
        int selected_y = selected + padding;
        int col = 0;
        while (col < cols - padding * 2) {
            wmove(win, y, col + padding);
            waddch(win, ' ');
            col += 1;
        }


        wmove(win, y, padding);
        waddstr(win, "[ ");
        if (todos[i]->status == Completed) {
            wmove(win, y, 1 + padding);
            waddstr(win, "x");
        }

        wmove(win, y, 2 + padding);
        waddstr(win, "]: ");

        wmove(win, y, 5 + padding);
        waddstr(win, todos[i]->data);

        if (selected == i) {
            wattroff(win, COLOR_PAIR(1));
        }

        wmove(win, selected_y, 1 + padding);

        i += 1;
    }

    wrefresh(win);
}
