#include <stdio.h>
#include "file_search.h"
#include "matcher.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <filename> <pattern>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *pattern  = argv[2];

    // Debug output for pattern inspection (optional)
    printf("Pattern: %s\n", pattern);

    // Search the file
    search_file(filename, pattern);

    return 0;
}
