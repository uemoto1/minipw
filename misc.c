#include "misc.h"

void get_key_value(char* p, char* k, char* v) {
    // Left hand side
    while (*p != '\0') {
        if (isspace(*p)) { p++; continue; }
        if (*p == '=') { p++; break; }
        *k++ = *p++;
    }
    *k = '\0';
    // Right hand side
    while (*p != '\0') {
        if (isspace(*p)) { p++; continue; }
        *v++ = *p++;
    }
    *v = '\0';
}

