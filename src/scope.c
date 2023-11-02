#include "../inc/scope.h"
#include "../inc/hash_table.h"
#include <stdlib.h>

struct scope *scope_create( int level, struct hash_table *hash_table, struct scope *prev, struct scope *next ) {
    struct scope *s = malloc(sizeof(struct scope));
    s->level = level;
    s->hash_table = hash_table;
    s->prev = prev;
    s->next = next;
    s->which = 0;
    return s;
}

// a new hash table to be pushed on the top of the stack - new scope
void scope_enter( struct scope *s ) {
    if (s == 0) return;

    struct hash_table *h = hash_table_create(0, 0);
    struct scope *new_scope = scope_create(s->level + 1, h, s, 0);
    s->next = new_scope;
}

// remove the topmost hash table
void scope_exit( struct scope *s ) {
    if (s == 0) return;

    if (s->prev != 0)
        s->prev->next = 0;

    hash_table_delete(s->hash_table);
    free(s);
}

// the numbers of hash tables in the current stack
int scope_level( struct scope *s ) {
    return s->level;
}

// add an entry to the topmost hash table of the stack
// map name to the symbol structure sym
int scope_bind( struct scope *s, const char *name, struct symbol *sym ) {
    int bind = hash_table_insert(s->hash_table, name, sym);
    if (bind == 1) {
        s->which++;
        return 0;
    }
    return 1;
}

// search thet stack of hash tables from top to bottom
// look for the first entry that matches name 
struct symbol *scope_lookup( struct scope *s, const char *name ) {
    if (s == 0) return 0;

    struct symbol *sym = hash_table_lookup(s->hash_table, name);
    if (sym == 0)
        sym = scope_lookup(s->prev, name);
    
    return sym;
}

// only search for the topmost table
struct symbol *scope_lookup_current( struct scope *s, const char *name ) {
    if (!s) return 0;
    struct symbol *sym = hash_table_lookup(s->hash_table, name);
    return sym;
}