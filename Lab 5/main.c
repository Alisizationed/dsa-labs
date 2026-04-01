#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "string_analysis.h"

int main() {
    char* string = get_sentence();
    print_sentence(string, "input.txt");
    int option;
    do {
        printf("0 - Exit\n");
        printf("1 - Display the number of declarative sentences\n");
        printf("2 - Display the length of the first sentence\n");
        printf("3 - Write all the data into a file\n");
        printf("Enter option: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                print_nr_declarative_sentences(nr_declarative_sentences(string));
                break;
            case 2:
                print_first_sentence_length(first_sentence_length(string));
                break;
            case 3:
                print_file(nr_declarative_sentences(string), first_sentence_length(string), "output.txt");
                break;
        }
    } while (option != 0);
    free(string);
    return 0;
}