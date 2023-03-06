#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "aids.h"

/**
 * Open a file in rw mode and create it if doesn't exist
 */
FILE* open_file_rw(const char *path) {
    struct stat st;
    int result = stat(path, &st);

    if (result == 0) {
        return fopen(path, "r+b");
    } else {
        return fopen(path, "w+b");
    }
}

char *read_file(FILE *fd) {
    fseek(fd, 0, SEEK_END);
    size_t length = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    char *buffer = malloc(length);

    if (buffer) {
        fread(buffer, 1, length, fd);
    }

    return buffer;
}

void write_file(FILE *fd, char *data) {
    fseek(fd, 0, SEEK_SET);
    int len = strlen(data);
    fwrite(data, 1, sizeof(char) * len, fd);
}

bool starts_with(const char *haystack, const char *needle) {
    size_t n = strlen(needle);

    return strncmp(haystack, needle, n) == 0;
}


size_t skip_while(bool (*predicate)(const char *), char *data) {
    size_t len = 0;

    while (*data != '\0' && predicate(data)) {
        len += 1;
        data += 1;
    }

    return len;
}
