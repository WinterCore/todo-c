#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "aids.h"
#include "todo.h"

int main() {
    char *directory = "./";
    char *file = ".data";
    char *path = malloc(sizeof(char) * 100);
    strcat(path, directory);
    strcat(path, file);

    FILE *fd = open_file_rw(path);
    char *data = read_file(fd);

    struct Todo *todos[100];
    size_t num_todos = 0;
    parse(todos[0], num_todos, data);


    return 0;
}

