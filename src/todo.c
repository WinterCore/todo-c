#include "todo.h"
#include "aids.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* getTodoStatusName(enum TodoStatus status) {
    switch (status) {
        case Todo: return "TODO";
        case Completed: return "COMPLETED";
    }

    UNREACHABLE
}

bool is_not_eol(const char *c) {
    return *c != '\n' && *c != EOF;
}

bool is_whitespace(const char *c) {
    return *c == ' ' || *c == '\n';
}

struct Todo* consume_todo(const char *todo_status_label, char **data) {
    struct Todo *todo = malloc(sizeof(Todo));

    size_t label_len = strlen(todo_status_label);
    *data += label_len + 2;
    size_t datalen = skip_while(is_not_eol, *data);
    char *todo_data = malloc(datalen + 1);
    strncpy(todo_data, *data, datalen);
    todo_data[datalen] = '\0';

    *data += datalen + 1;
    todo->data = todo_data;
    todo->status = Todo;

    return todo;
}

struct Todo* parse_todo(char **data) {
    
    const char *todo_status = getTodoStatusName(Todo);
    const char *completed_status = getTodoStatusName(Completed);

    // Skip whitespace
    data += skip_while(is_whitespace, *data);

    if (starts_with(*data, todo_status)) {
        return consume_todo(todo_status, data);
    } else if (starts_with(*data, completed_status)) {
        return consume_todo(completed_status, data);
    }

    fprintf(stderr, "ERROR: CORRUPT DATA FILE\n");
    UNREACHABLE
}

const size_t BUFFER_SIZE = 100;

struct Todo** parse(size_t *num_todos, char *data) {
    struct Todo **result = malloc(0);
    struct Todo **buffer = malloc(sizeof(struct Todo*) * BUFFER_SIZE);
    size_t i = 0;

    while (*data != '\0') {
        struct Todo *todo = parse_todo(&data);

        buffer[i % BUFFER_SIZE] = todo;

        i += 1;

        if (i != 0 && i % BUFFER_SIZE == 0) {
            size_t chunk = i / BUFFER_SIZE;
            result = realloc(result, sizeof(struct Todo*) * BUFFER_SIZE * chunk);
            memcpy(result + i - BUFFER_SIZE, buffer, chunk);
        }
    }


    if (i % BUFFER_SIZE > 0) {
        size_t new_size = (sizeof(struct Todo*) * BUFFER_SIZE * (i / BUFFER_SIZE)) +
                          (sizeof(struct Todo*) * (i % BUFFER_SIZE));


        result = realloc(result, new_size);
        memcpy(
            result + (i / BUFFER_SIZE) * BUFFER_SIZE,
            buffer,
            sizeof(struct Todo*) * (i % BUFFER_SIZE)
        );
    }


    *num_todos = i;
    free(buffer);

    return result;
}

struct Todo* create_todo(char *str) {
    struct Todo *todo = malloc(sizeof(Todo));
    todo->status = Todo;
    todo->data = str;
    return todo;
}

void delete_todo(struct Todo* todo) {
    free(todo);
}
