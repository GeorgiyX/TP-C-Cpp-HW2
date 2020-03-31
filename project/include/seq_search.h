#ifndef TP_C_CPP_HW2_SEQ_SEARCH_H
#define TP_C_CPP_HW2_SEQ_SEARCH_H
// Public API
typedef struct {
    size_t sequence_count;
    const char **sequence_vector;
} sequences_vector;

sequences_vector *search_sequences(const char *file_path, size_t sequences_cnt, const char **in_sequences);
void free_sequences_vector(sequences_vector *vector);

#endif //TP_C_CPP_HW2_SEQ_SEARCH_H
