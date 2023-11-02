
#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"

struct symbol;

struct scope {
    int level;
    struct hash_table *hash_table;
    struct scope *prev;
    struct scope *next;
    int which; // ordinal position of local variables and parameters
};

struct scope *scope_create( int level, struct hash_table *hash_table, struct scope *prev, struct scope *next );

void scope_enter( struct scope *s );
void scope_exit( struct scope *s );
int scope_level( struct scope *s );

int scope_bind( struct scope *s, const char *name, struct symbol *sym );
struct symbol *scope_lookup( struct scope *s, const char *name );
struct symbol *scope_lookup_current( struct scope *s, const char *name );

#endif
