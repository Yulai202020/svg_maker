#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdarg.h>
#include "string.c"

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
    } else if (strcmp(type, "rect") == 0) {
        char* x1 = readline("Input x1: ");
        char* y1 = readline("Input y1: ");
        char* x2 = readline("Input x2: ");
        char* y2 = readline("Input y2: ");
        char* style = readline("Input style: ");

        if (!x1 || !x2 || !y1 || !y2 || !style) {
            fprintf(stderr, "Error reading input values\n");
            fclose(fptr);
            free_all(type, x1, x2, y1, y2, style, filename, NULL);
            return 1;
        }

        if (strcmp(style, "") == 0) {
            snprintf(tag, MAX_LINE_LENGTH, "<rect x1=\"%s\" y1=\"%s\" x2=\"%s\" y2=\"%s\"/>", x1, y1, x2, y2);
        } else {
            snprintf(tag, MAX_LINE_LENGTH, "<rect x1=\"%s\" y1=\"%s\" x2=\"%s\" y2=\"%s\" style=\"%s\"/>", x1, y1, x2, y2, style);
        }
    } else if (strcmp(type, "ellipse") == 0) {
        char* x = readline("Input x: ");
        char* y = readline("Input y: ");
        char* rx = readline("Input radius by x: ");
        char* ry = readline("Input radius by ys: ");
        char* style = readline("Input style: ");

        if (!x || !y || !ry || !rx || !style) {
            fprintf(stderr, "Error reading input values\n");
            fclose(fptr);
            free_all(type, x, y, ry, rx, style, filename, NULL);
            return 1;
        }
    
        if (strcmp(style, "") == 0) {
            snprintf(tag, MAX_LINE_LENGTH, "<ellipse cx=\"%s\" cy=\"%s\" rx=\"%s\" ry=\"%s\"/>", x, y, rx, ry);
        } else {
            snprintf(tag, MAX_LINE_LENGTH, "<ellipse cx=\"%s\" cy=\"%s\" rx=\"%s\" ry=\"%s\" style=\"%s\"/>", x, y, rx, ry, style);
        }

        free_all(type, x, y, ry, rx, style, NULL);
    } else if (strcmp(type, "polyline") == 0) {
        char** all = (char**) malloc(1024*sizeof(char*));

        char* x = "a";
        char* y = "a";
        int count = 0;

        while (strcmp(x, "") != 0 && strcmp(y, "") != 0) {
            x = readline("Input first point (press enter to leave): ");
            if (strcmp(x, "") == 0) {
                break;
            }
            y = readline("Input second point: ");
            if (strcmp(y, "") == 0) {
                break;
            }
            x = stringSum(x, ",");
            x = stringSum(x, y);
            all[count] = x;
            count++;
        }

        if (!x || !y) {
            fprintf(stderr, "Error reading input values\n");
            fclose(fptr);

            for (int i = 0; i < count; i++) {
                free(all[i]);
            }

            free(all);

            free_all(type, x, y, filename, NULL);
            return 1;
        }

        char* style = readline("Input style: ");

        if (strcmp(style, "") == 0) {
            snprintf(tag, MAX_LINE_LENGTH, "<polyline points=\"%s\" style=\"fill: none;stroke: #ff0000;\"/>", join(all, count, " "));
        } else {
            snprintf(tag, MAX_LINE_LENGTH, "<polyline points=\"%s\" style=\"%s\"/>", join(all, count, " "), style);
        }

        free_all(type, x, y, NULL);

        for (int i = 0; i < count; i++) {
            free(all[i]);
        }

        free(all);
    } else if (strcmp(type, "polygon") == 0) {
        char** all = (char**) malloc(1024*sizeof(char*));

        char* x = "a";
        char* y = "a";
        int count = 0;

        while (strcmp(x, "") != 0 && strcmp(y, "") != 0) {
            x = readline("Input first point (press enter to leave): ");
            if (strcmp(x, "") == 0) {
                break;
            }
            y = readline("Input second point: ");
            if (strcmp(y, "") == 0) {
                break;
            }
            x = stringSum(x, ",");
            x = stringSum(x, y);
            all[count] = x;
            count++;
        }

        if (!x || !y) {
            fprintf(stderr, "Error reading input values\n");
            fclose(fptr);
            for (int i = 0; i < count; i++) {
                free(all[i]);
            }

            free(all);
            free_all(type, x, y, filename, NULL);
            return 1;
        }

        char* style = readline("Input style: ");

        if (strcmp(style, "") == 0) {
            snprintf(tag, MAX_LINE_LENGTH, "<polygon points=\"%s\" style=\"fill: none;stroke: #ff0000;\"/>", join(all, count, " "));
        } else {
            snprintf(tag, MAX_LINE_LENGTH, "<polygon points=\"%s\" style=\"%s\"/>", join(all, count, " "), style);
        }

        free_all(type, x, y, NULL);
        for (int i = 0; i < count; i++) {
            free(all[i]);
        }

        free(all);
    } else {
        free_all(output, tag, filename);
        fclose(fptr);
        return 1;
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
