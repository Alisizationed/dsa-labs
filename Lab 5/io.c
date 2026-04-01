#include <stdio.h>
#include <stdlib.h>

char* get_sentence() {
    char* sentence = (char*) malloc(sizeof(char) * 200);
    printf("Enter sentence: ");
    fgets(sentence, 199, stdin);
    return sentence;
}

void print_sentence(char* str, char* file_name) {
    FILE* f = fopen(file_name, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(f, "%s", str);
    fclose(f);
}

void print_first_sentence_length(int n) {
    printf("The length of the first sentence is %d.\n", n);
}

void print_nr_declarative_sentences(int nr) {
    printf("The number of declarative sentences is %d.\n", nr);
}

void print_file(int nr, int length, char* file_name) {
    FILE* f = fopen(file_name, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(f,"The number of the declarative sentences is %d.\n", nr);
    fprintf(f,"The length of the first sentence is %d.\n", length);
    fclose(f);
}