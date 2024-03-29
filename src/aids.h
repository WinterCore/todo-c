#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define UNIMPLEMENTED \
    fprintf(stderr, "UNIMPLEMENTED"); \
    exit(EXIT_FAILURE); \

#define UNREACHABLE \
    fprintf(stderr, "UNREACHABLE"); \
    exit(EXIT_FAILURE); \

#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define MIN(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
    _a > _b ? _b : _a; })

#ifndef AIDS
#define AIDS

FILE* open_file_rw(const char *path);

char *read_file(FILE *fd);

bool starts_with(const char *haystack, const char *needle);

size_t skip_while(bool (*predicate)(const char *), char *data);

#endif
