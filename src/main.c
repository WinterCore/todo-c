#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main() {
    char *directory = "";
    char *file = ".data";
    char *path = malloc(sizeof(char) * 100);
    strcat(path, directory);
    strcat(path, file);
    FILE *fd = fopen(path, "w");


    return 0;
}

