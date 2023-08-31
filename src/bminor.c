#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/encoder.h"  

void print_usage() {
    printf("usage: bminor --encode input.bminor\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage();
        return 1;
    }

    char input[1280];
    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        perror("error opening file");
        return 1;
    }

    fgets(input, sizeof(input), file);
    fclose(file);

    if (strcmp(argv[1], "--encode") == 0) {
        char decoded[1280];
        char encoded[1280];

        if (string_decode(input, decoded) == 0) {
            //printf("decoded: %s\n", decoded);
            if (string_encode(decoded, encoded) == 0) printf("%s\n", encoded);            
            else {
                printf("encoding failed.\n");
                return 1;
            }
        }
        else return 1;
    }
    return 0;
}
