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

double strtod_fortran(char *s) {
    for (int i=0; i<64 && s[i] != '\0'; i++) {
        if (s[i] == 'd' || s[i] == 'D')
            s[i] = 'e';
    }
    return strtod(s, NULL);
}

void next_line(FILE* fp) {
    fscanf(fp, "%*[^\n]%*c");
}

int read_int(FILE* fp) {
    char tmp[32+1];
    fscanf(fp, "%32s", tmp);
    return strtol(tmp, NULL, 10);
}

double read_real(FILE* fp) {
    char tmp[32+1];
    fscanf(fp, "%32s", tmp);
    return strtod_fortran(tmp);
}

