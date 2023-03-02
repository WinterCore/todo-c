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

struct Todo* parse_todo(char *data);

void parse(struct Todo todos[], size_t num_todos, char *data);

#endif
