#include "todo.h"
#include "aids.h"
#include <string.h>

const char* getTodoStatusName(enum TodoStatus status) {
    switch (status) {
        case Todo: return "TODO";
        case Completed: return "COMPLETED";
    }
}

struct Todo* parse_todo(char *data) {
    struct Todo *todo = malloc(sizeof(Todo));
    
    const char *todoStatus = getTodoStatusName(Todo);
    const char *completedStatus = getTodoStatusName(Completed);
    printf("Here %s", data);

    if (starts_with(data, todoStatus)) {
        size_t len = strlen(todoStatus);
        data += len + 2;
        size_t datalen = strlen(data);
        char *data = malloc(datalen);

        todo->data = data;
        todo->status = Todo;
    } else if (starts_with(data, completedStatus)) {
        size_t len = strlen(completedStatus);
        data += len + 2;
        size_t datalen = strlen(data);
        char *data = malloc(datalen);

        todo->data = data;
        todo->status = Todo;
    } else {
        UNIMPLEMENTED
    }

    return todo;
}

void parse(struct Todo todos[], size_t num_todos, char *data) {
    while (data != NULL) {
        struct Todo *todo = parse_todo(data);

        printf("TODO: %s", todo->data);
    }
}
