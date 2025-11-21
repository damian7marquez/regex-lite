#include "matcher.h"

// Forward declaration
static int match_here(const char *pattern, const char *text);

int match(const char *pattern, const char *text) {

    if (pattern[0] == '^') {
        return match_here(pattern + 1, text);
    }

    do {
        if (match_here(pattern, text))
            return 1;
    } while (*text++ != '\0');

    return 0;
}

static int match_here(const char *pattern, const char *text) {

    // Case 1: end of pattern → success
    if (pattern[0] == '\0')
        return 1;

    // Case 2: $ at end of pattern → match end of text
    if (pattern[0] == '$' && pattern[1] == '\0')
        return *text == '\0';

    // Case 3: literal match
    if (pattern[0] == text[0] && text[0] != '\0')
        return match_here(pattern + 1, text + 1);

    return 0;
}
