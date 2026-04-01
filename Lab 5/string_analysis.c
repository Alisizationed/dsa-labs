int nr_declarative_sentences(char* str) {
    int nr = 0;
    for (char* c = str; *c != '\0'; c++) {
        if (*c == '.') nr++;
    }
    return nr;
}

int first_sentence_length(char* str) {
    int nr = 0;
    for (char* c = str; *c != '.'; c++, nr++) {}
    return nr;
}