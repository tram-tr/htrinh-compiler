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
struct scope * scope_enter( struct scope *s ) {
    struct hash_table *h = hash_table_create(0, 0);
    struct scope *new_scope = scope_create(s->level + 1, h, s, 0);
    new_scope->next = s;
    return new_scope;
}

// remove the topmost hash table
struct scope * scope_exit( struct scope *s ) {
    struct scope *head = s->next;
    hash_table_delete(s->hash_table);
    //free(s);
    return head;
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
        //printf("%d\n", s->which);
        s->which++;
        return 0;
    }
    struct symbol *t = scope_lookup(s, name); 
    if (sym->type->kind != TYPE_FUNCTION) {
        printf("resolve error: %s is already defined\n", sym->name);
        return 1;
    }
    else if (type_compare(sym->type, t->type) == 1) {
        printf("resolve error: type %s does not match\n", sym->name);
        return 1;
    }
    return 0;
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