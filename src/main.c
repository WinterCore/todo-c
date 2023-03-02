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
    use_default_colors();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_CYAN);

    render(todos, num_todos, selected);
    
    sleep(5);

    endwin();

    return 0;
}

