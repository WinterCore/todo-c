#ifndef TODO
#define TODO

#define MAX_TODO_TEXT_LEN 200

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "hector.h"
#include "aids.h"


enum TodoStatus {
    Todo,
    Completed,
};

struct Todo {
    enum TodoStatus status;
    char *data;
};

const char *getTodoStatusName(enum TodoStatus status);

struct Todo *parse_todo(char **data);

struct Hector *read_todos(FILE *fd);
void write_todos(FILE *fd, struct Hector *todos);

struct Hector *filter_todos(struct Hector *todos, enum TodoStatus status);

struct Todo *create_todo(char *str, enum TodoStatus status);

void toggle_todo_status(struct Todo* todo);
void delete_todo(struct Todo* todo);

#endif
