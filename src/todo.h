#ifndef TODO
#define TODO

#include <stdbool.h>


enum TodoStatus {
    Todo,
    Completed,
};

struct Todo {
    TodoStatus status;
    char *data;
};

#endif
