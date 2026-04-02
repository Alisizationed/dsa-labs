_Bool is_alphanumeric(char c) {
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

int nr_declarative_sentences(char* str) {
    int nr = 0;
    _Bool flag = 0;
    for (char* c = str; *c != '\0'; c++) {
        if (is_alphanumeric(*c)) flag = 1;
        if (*c == '.' && flag) {
            nr++;
            flag = 0;
        }
    }
    return nr;
}

int first_sentence_length(char* str) {
    int nr = 0;
    _Bool flag = 0;
    for (char* c = str; *c != '.' || !flag; c++, nr++) {
        if (is_alphanumeric(*c)) flag = 1;
        if (*c == '.') {
            nr = 0;
        }
        if (c != str && !is_alphanumeric(*c) && !is_alphanumeric(*(c - 1))) {
            nr--;
        }
    }
    return nr;
}
// The first letter A. The second N. The third is A. The fourth is S. The fifth is T. The sixth is A. The seventh is S. The eighth is I. The nineth is A.
// gfdhdh. ghfjfj. .. hfjfgj.