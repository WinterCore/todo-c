#ifndef TODO
#define TODO

#include <stdbool.h>
#include <string.h>


enum TodoStatus {
    Todo,
    Completed,
};

struct Todo {
    enum TodoStatus status;
    char *data;
};

const char* getTodoStatusName(enum TodoStatus status);

struct Todo* parse_todo(char **data);

struct Todo** parse(size_t *num_todos, char *data);

struct Todo* create_todo(char *str);

#endif
