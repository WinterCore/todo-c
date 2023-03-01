#include "aids.h"
#include <stdio.h>
#include <cstddef>

char *read_file(FILE *fd) {
    fseek(fd, 0, SEEK_END);
    size_t length = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    char *buffer = malloc(length);

    if (buffer) {
        fread(fd, 1, length, fd);
    }

    return buffer;
}
