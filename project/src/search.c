#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

void free_founded_sequence(founded_sequence *node_to_remove) {
    while (node_to_remove) {
        founded_sequence *next_node = node_to_remove->next;
        free(node_to_remove);
        node_to_remove = next_node;
    }
}


int is_sequence_in_data(char *data, const char *sequence) {
    if (!data || !sequence) {
        return 0;
    }
    int result = 1;
    while (*sequence) {
        const char ch[2] = { *sequence++, '\0'};
        if (!(data = strstr(data, ch))) {
            result = 0;
            break;
        }
    }
    return result;
}

int get_mmap_data(const char *path, char **data, size_t *file_size) {
    if (!path || !data || !file_size) {
        return -1;
    }

    int file_desc = 0;
    if ((file_desc = open(path, O_RDONLY)) == -1) {
        return -1;
    }

    struct stat stat_buffer = {};
    if (fstat(file_desc, &stat_buffer)) {
        return -1;
    }

    *data = NULL;
    *file_size = stat_buffer.st_size;
    *data = (char *) mmap(NULL, stat_buffer.st_size, PROT_READ, MAP_SHARED, file_desc, 0);
    if (*data == MAP_FAILED) {
        return -1;
    }
    return close(file_desc);
}

void free_sequences_vector(sequences_vector *vector) {
    if (!vector) {
        return;
    }
    free(vector->sequence_vector);
    free(vector);
}
