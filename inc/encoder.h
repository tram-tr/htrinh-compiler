#ifndef ENCODER_H
#define ENCODER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int string_encode(const char *s, char *es);
int string_decode(const char *es, char *s);
int string_compare(const char *es, const char *s);

#endif