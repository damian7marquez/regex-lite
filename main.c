#include <stdio.h>
#include "file_search.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <pattern>\n", argv[0]);
        return 1;
    }

    search_file(argv[1], argv[2]);
    return 0;
}
