#include "matcher.h"

/* forward declarations */
static int match_here(const char *pattern, const char *text);
static int match_class(const char *pattern, char c, int *len);
static int match_one(const char *pattern, char c, int *len);

/* match_class: checks things like [abc] or [a-z] */
/* it looks inside the [ ] and decides if the character fits */
static int match_class(const char *pattern, char c, int *len) {
    int i = 1;              /* skip the first '[' */
    int matched = 0;        /* this becomes 1 if the character fits */

    /* go through everything between [ and ] */
    while (pattern[i] != ']' && pattern[i] != '\0') {

        /* handle ranges like [a-z] */
        if (pattern[i+1] == '-' && pattern[i+2] != ']' && pattern[i+2] != '\0') {
            char start = pattern[i];
            char end   = pattern[i+2];

            /* check if c is between start and end */
            if (c >= start && c <= end)
                matched = 1;

            i += 3;     /* skip "a-z" */
            continue;
        }

        /* handle single characters like [abc] */
        if (pattern[i] == c)
            matched = 1;

        i++;
    }

    *len = i + 1;       /* the total length of the class, including ']' */
    return matched;
}

/* match_one: checks ONE pattern piece */
/* this can be a single letter OR a whole [class] */
static int match_one(const char *pattern, char c, int *len) {

    /* if it starts with [ then it is a class */
    if (pattern[0] == '[') {
        return match_class(pattern, c, len);
    }

    /* otherwise it is just a normal character */
    *len = 1;
    return pattern[0] == c;
}

/* match: the function that starts everything */
/* if pattern starts with ^ we ONLY match at the beginning */
/* otherwise we try every position in the text */
int match(const char *pattern, const char *text) {

    /* pattern begins with ^ means: must match at the very start */
    if (pattern[0] == '^')
        return match_here(pattern + 1, text);

    /* try matching starting at every text position */
    do {
        if (match_here(pattern, text))
            return 1;
    } while (*text++ != '\0');

    return 0;
}

/* match_here: the main matcher function */
/* it tries to match the pattern step by step */
static int match_here(const char *pattern, const char *text) {

    /* if we finished the pattern, it matched successfully */
    if (pattern[0] == '\0')
        return 1;

    /* if pattern ends in $, text must also be finished */
    if (pattern[0] == '$' && pattern[1] == '\0')
        return *text == '\0';

    /* X? = this letter is optional */
    if (pattern[1] == '?') {

        int len = 0;
        match_one(pattern, text[0], &len);

        /* try using the character */
        if (match_one(pattern, text[0], &len) && text[0] != '\0') {
            if (match_here(pattern + len + 1, text + 1))
                return 1;
        }

        /* or try skipping it */
        return match_here(pattern + len + 1, text);
    }

    /* X* = this letter can repeat many times (or zero times) */
    {
        int len = 0;
        match_one(pattern, text[0], &len);

        /* if the next character in pattern is * */
        if (pattern[len] == '*') {

            /* case 1: try skipping all the repeated letters */
            if (match_here(pattern + len + 1, text))
                return 1;

            /* case 2: try using 1 or more repeated letters */
            int i = 0;
            while (text[i] != '\0' &&
                   match_one(pattern, text[i], &len)) {

                if (match_here(pattern + len + 1, text + i + 1))
                    return 1;

                i++;
            }

            return 0;
        }
    }

    /* normal match: literal or class */
    {
        int len = 0;

        /* if one character matches, move to the next */
        if (match_one(pattern, text[0], &len) && text[0] != '\0')
            return match_here(pattern + len, text + 1);
    }

    /* if nothing matched, fail */
    return 0;
}
