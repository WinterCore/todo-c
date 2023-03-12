#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hector.h"
#include "todo.h"

const char* get_todo_status_name(enum TodoStatus status) {
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

char *consume_todo_text(const char *todo_status_label, char **data) {
    size_t label_len = strlen(todo_status_label);
    *data += label_len + 2;
    size_t datalen = skip_while(is_not_eol, *data);
    char *todo_data = malloc(datalen + 1);
    strncpy(todo_data, *data, datalen);
    todo_data[datalen] = '\0';

    *data += datalen + 1;

    return todo_data;
}

struct Todo* parse_todo(char **data) {
    
    const char *todo_status = get_todo_status_name(Todo);
    const char *completed_status = get_todo_status_name(Completed);

    // Skip whitespace
    data += skip_while(is_whitespace, *data);

    if (starts_with(*data, todo_status)) {

        char *text = consume_todo_text(todo_status, data);
        return create_todo(text, Todo);

    } else if (starts_with(*data, completed_status)) {

        char *text = consume_todo_text(completed_status, data);
        return create_todo(text, Completed);

    }

    fprintf(stderr, "ERROR: CORRUPT DATA FILE\n");
    UNREACHABLE
}


struct Hector *read_todos(FILE *fd) {
    char *data_ptr = read_file(fd);
    char *data = data_ptr;

    struct Hector *hec = hector_create(sizeof(struct Todo *), 5);

    while (*data != '\0') {
        struct Todo *todo = parse_todo(&data);

        hector_push(hec, todo);
    }

    free(data_ptr);
    return hec;
}

struct Hector *filter_todos(struct Hector *todos, enum TodoStatus status) {
    struct Hector *result = hector_create(sizeof(struct Todo *), 1);

    size_t len = hector_size(todos);
    for (size_t i = 0; i < len; i += 1) {
        struct Todo *todo = hector_get(todos, i);
        if (todo->status == status) {
            hector_push(result, todo);
        }
    }

    return result;
}

void write_todos(FILE *fd, struct Hector *todos) {
    size_t len = hector_size(todos);
    char *buffer = malloc(sizeof(char) * (MAX_TODO_TEXT_LEN + 50) * len);

    int p = 0;
    for (size_t i = 0; i < len; i += 1) {
        struct Todo *todo = hector_get(todos, i);
        const char *label = get_todo_status_name(todo->status);
        int label_len = strlen(label);

        strncpy(p + buffer, label, label_len);
        strcpy(p + buffer + label_len, ": ");
        int text_len = strlen(todo->data);
        strcpy(p + buffer + label_len + 2, todo->data);
        buffer[p + label_len + 2 + text_len] = '\n';
        p += label_len + 2 + text_len + 1;
    }

    fwrite(buffer, 1, p, fd);
    free(buffer);
}

struct Todo* create_todo(char *str, enum TodoStatus status) {
    struct Todo *todo = malloc(sizeof(Todo));
    todo->status = status;
    todo->data = str;
    return todo;
}

void toggle_todo_status(struct Todo* todo) {
    switch (todo->status) {
        case Todo: {
           todo->status = Completed;
           break;
        };
        case Completed: {
           todo->status = Todo;
           break;
        };
        default: {
            UNREACHABLE
            break;
        };
    }

}

void delete_todo(struct Todo* todo) {
    free(todo->data);
    free(todo);
}
