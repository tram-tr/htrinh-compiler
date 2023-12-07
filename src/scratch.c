#include "../inc/scratch.h"

int regs_scratch [7]; 
char * reg_names[7] = {"%rbx", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};
char * arg_names[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
int label_count = 0;

int scratch_alloc() {
    for (int i = 0; i < 7; i++) {
        if (regs_scratch[i] == 0) {
            regs_scratch[i] = 1;
            return i;
        }
    }
    printf("out of registers.\n\n");
    return -1;
}

void scratch_free( int r ) {
    regs_scratch[r] = 0;
}

const char * scratch_name( int r ) {
    if (r >= 7)
        return 0;

    return reg_names[r];
}

const char * arg_name ( int r ) {
    if (r >= 6) 
        fprintf(stdout, "cannot have more than 6 registers\n");
    
    return arg_names[r];
}

int label_create() {
    return label_count++;
}

const char * label_name( int label ) {
    char s[10];
    snprintf(s, 10, ".L%d", label);
    return strdup(s);
}
