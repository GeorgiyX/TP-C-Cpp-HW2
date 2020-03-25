#include <stdio.h>
#include <bits/types/FILE.h>
#include "utils.h"


int write_to_file(const char *path, double data) {
    FILE *out_file = fopen(path, "a+");
    if (out_file == NULL) {
        return -1;
    }
    fprintf(out_file, "%f\n", data);
    fclose(out_file);
    return 0;
}
