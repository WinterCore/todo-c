#include "aids.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

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

bool starts_with(const char *haystack, const char *needle) {
    size_t n = strlen(needle);

    return strncmp(haystack, needle, n) == 0;
}
