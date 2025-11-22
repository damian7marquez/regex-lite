#include "matcher.h"

// Forward declarations
static int match_here(const char *pattern, const char *text);
static int match_star(char c, const char *pattern, const char *text);
static int match_set(const char *pattern, const char *text, int *jump);

// Entry point
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

// Recursive matcher
static int match_here(const char *pattern, const char *text) {

    // End of pattern => match
    if (*pattern == '\0')
        return 1;

    // Handle $
    if (pattern[0] == '$' && pattern[1] == '\0')
        return *text == '\0';

    // Handle character set [abc]
    if (pattern[0] == '[') {
        int jump = 0;

        if (*text == '\0')
            return 0;

        if (match_set(pattern, text, &jump)) {
            return match_here(pattern + jump, text + 1);
        }

        return 0;
    }

    // Handle x*
    if (pattern[1] == '*') {
        return match_star(pattern[0], pattern + 2, text);
    }

    // Handle x?
    if (pattern[1] == '?') {

        // Case 1 — use the character
        if (*text != '\0' && pattern[0] == text[0]) {
            if (match_here(pattern + 2, text + 1))
                return 1;
        }

        // Case 2 — skip the optional character
        return match_here(pattern + 2, text);
    }

    // Literal match
    if (*text != '\0' && pattern[0] == text[0]) {
        return match_here(pattern + 1, text + 1);
    }

    return 0;
}

// Handle x*
static int match_star(char c, const char *pattern, const char *text) {

    // Try zero repetitions
    if (match_here(pattern, text))
        return 1;

    // Consume as many matching chars as possible
    while (*text != '\0' && *text == c) {
        text++;
        if (match_here(pattern, text))
            return 1;
    }

    return 0;
}

// Handle [abc] or [a-z]
static int match_set(const char *pattern, const char *text, int *jump) {

    int i = 1;
    int matched = 0;

    while (pattern[i] != ']' && pattern[i] != '\0') {

        // Handle range a-z
        if (pattern[i+1] == '-' && pattern[i+2] != ']' && pattern[i+2] != '\0') {

            char start = pattern[i];
            char end   = pattern[i+2];

            if (*text >= start && *text <= end) {
                matched = 1;
            }

            i += 3;
        }

        else {
            // Single character
            if (pattern[i] == *text) {
                matched = 1;
            }
            i++;
        }
    }

    // jump past the closing ']'
    *jump = i + 1;

    return matched;
}
