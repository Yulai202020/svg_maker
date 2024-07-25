#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdarg.h>

#define MAX_LINE_LENGTH 1024

void free_all(void* first, ...) {
    va_list args;
    va_start(args, first);

    void* ptr = first;
    while (ptr != NULL) {
        free(ptr);
        ptr = va_arg(args, void*);
    }

    va_end(args);
}

int main(int argc, char** argv) {
    char* output = (char*) malloc(MAX_LINE_LENGTH);
    char* tag = (char*) malloc(MAX_LINE_LENGTH);

    if (!tag || !output) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    char* filename = readline("Input filename: ");
    if (!filename) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }

    FILE* fptr = fopen(filename, "w");
    if (!fptr) {
        perror("Error opening file");
        free(filename);
        return 1;
    }

    char* type = readline("Input type (circle or rect): ");
    if (!type) {
        fprintf(stderr, "Error reading type\n");
        fclose(fptr);
        free(filename);
        return 1;
    }

    if (strcmp(type, "circle") == 0) {
        char* center_x = readline("Input x: ");
        char* center_y = readline("Input y: ");
        char* radius = readline("Input radius: ");
        char* style = readline("Input style: ");

        if (!center_x || !center_y || !radius || !style) {
            fprintf(stderr, "Error reading input values\n");
            fclose(fptr);
            free_all(type, center_x, center_y, radius, style, filename, NULL);
            return 1;
        }

        if (strcmp(style, "") == 0) {
            snprintf(tag, MAX_LINE_LENGTH, "<circle cx=\"%s\" cy=\"%s\" r=\"%s\"/>", center_x, center_y, radius);
        } else {
            snprintf(tag, MAX_LINE_LENGTH, "<circle cx=\"%s\" cy=\"%s\" r=\"%s\" style=\"%s\"/>", center_x, center_y, radius, style);
        }

        free_all(center_x, center_y, radius, style, NULL);
    }

    // Format the output
    snprintf(output, MAX_LINE_LENGTH, "<svg>\n\t%s\n</svg>", tag);
    fprintf(fptr, "%s", output);

    // Clean up
    free_all(tag, type, output, filename, NULL);

    // close file
    fclose(fptr);

    return 0;
}
