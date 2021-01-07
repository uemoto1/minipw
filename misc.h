#pragma once
#include "common.h"

uint32_t rand_xorshift(void);
void get_key_value(char *, char *, char *);
double strtod_fortran(char *);

void next_line(FILE *);
int read_int(FILE *);
double read_real(FILE *);
