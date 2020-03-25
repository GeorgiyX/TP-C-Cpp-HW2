#ifndef TP_C_CPP_HW2_SEQ_SEARCH_H
#define TP_C_CPP_HW2_SEQ_SEARCH_H
typedef struct founded_sequence founded_sequence;
struct founded_sequence {
    const char *sequence;
    founded_sequence *next;
};

founded_sequence *search_sequences(const char *file_path, size_t sequences_cnt, const char **in_sequences);
void free_founded_sequence(founded_sequence *node);

#endif //TP_C_CPP_HW2_SEQ_SEARCH_H
