#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "seq_search.h"

void free_founded_sequence(founded_sequence *node_to_remove) {
    if (!node_to_remove) {
        return;
    }
    while (node_to_remove) {
        founded_sequence *next_node = node_to_remove->next;
        free(node_to_remove);
        node_to_remove = next_node;
    }
}


int is_sequence_in_data(char *data, const char *sequence) {
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

int get_mmap_data(const char *path, char **data) {
    if (!path || !data) {
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
    *data = (char *) mmap(NULL, stat_buffer.st_size, PROT_READ, MAP_SHARED, file_desc, 0);
    if (*data == MAP_FAILED) {
        return -1;
    }
    return close(file_desc);
}
