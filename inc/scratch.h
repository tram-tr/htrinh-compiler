
#ifndef SCRATCH_H
#define SCRATCH_H

#include <stdio.h>
#include <string.h>

int scratch_alloc();
void scratch_free( int r );
const char * scratch_name( int r );
const char * arg_name ( int r );
int label_create();
const char * label_name( int label );

#endif
