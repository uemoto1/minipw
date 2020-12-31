#include "inputparam.h"

void read_inputparam(FILE* fp, Param* p) {
    char key[256], value[256];
    // Variables for getline
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int i = 0;

    while (1) {
        i++;
        nread = getline(&line, &len, fp);
        if (nread < 0) break; // End of file
        if (nread >= 255) { printf("# ERROR(L%d): too long line!\n", i); exit(-1); }

        get_key_value(line, key, value);
        // Skip empty line
        if (strlen(key) == 0) continue;
        // Assign to buffer
        if (strcmp(key, "a1_x") == 0) { p->a1_x = atof(value); continue; }
        if (strcmp(key, "a1_y") == 0) { p->a1_y = atof(value); continue; }
        if (strcmp(key, "a1_z") == 0) { p->a1_z = atof(value); continue; }
        if (strcmp(key, "a2_x") == 0) { p->a2_x = atof(value); continue; }
        if (strcmp(key, "a2_y") == 0) { p->a2_y = atof(value); continue; }
        if (strcmp(key, "a2_z") == 0) { p->a2_z = atof(value); continue; }
        if (strcmp(key, "a3_x") == 0) { p->a3_x = atof(value); continue; }
        if (strcmp(key, "a3_y") == 0) { p->a3_y = atof(value); continue; }
        if (strcmp(key, "a3_z") == 0) { p->a3_z = atof(value); continue; }
        if (strcmp(key, "ecut") == 0) { p->ecut = atof(value); continue; }
        if (strcmp(key, "nk1") == 0) { p->nk1 = atoi(value); continue; }
        if (strcmp(key, "nk2") == 0) { p->nk2 = atoi(value); continue; }
        if (strcmp(key, "nk3") == 0) { p->nk3 = atoi(value); continue; }
        if (strcmp(key, "nb") == 0) { p->nb = atoi(value); continue; }

        printf("# ERROR(L%d): unknown keyword '%s'!\n", i, key);
        exit(-1);
    }
}
