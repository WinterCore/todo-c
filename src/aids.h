#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define UNIMPLEMENTED \
    fprintf(stderr, "UNIMPLEMENTED"); \
    exit(EXIT_FAILURE); \

#ifndef AIDS
#define AIDS

FILE* open_file_rw(const char *path);

char *read_file(FILE *fd);

bool starts_with(const char *haystack, const char *needle);

#endif
