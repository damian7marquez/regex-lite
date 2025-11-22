#include <stdio.h>
#include <string.h>
#include "file_search.h"
#include "matcher.h"

#define MAX_LINE 1024

void search_file(const char *filename, const char *pattern)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Sorry, your file named %s, did not open.\n", filename);
        return;
    }

    char line[MAX_LINE];
    int line_number = 1;

    while (fgets(line, MAX_LINE, fp) != NULL)
    {
        // Remove trailing newline character (VERY IMPORTANT)
        line[strcspn(line, "\n")] = '\0';

        // Run the matcher
        if (match(pattern, line))
        {
            printf("%d: %s\n", line_number, line);
        }

        line_number++;
    }

    fclose(fp);
}
