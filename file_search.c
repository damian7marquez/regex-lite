#include <stdio.h>
#include <string.h>
#include "file_search.h"
#include "matcher.h"

#define MAX_LINE 1024  // max characters in one line

void search_file(const char *filename, const char *pattern) {

    // Step 1: try to open the file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    int line_number = 1;

    // Step 2: read the file line by line
    while (fgets(line, MAX_LINE, fp) != NULL) {

        // Step 3: pass line to the matcher
        if (match(pattern, line)) {

            // Step 4: print line number and the line itself
            printf("%d: %s", line_number, line);
        }

        line_number++;
    }

    fclose(fp);
}
