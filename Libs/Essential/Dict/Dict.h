#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dict_node {
    struct Dict_node *next;
    struct Dict_node *prev;
    char *key;
    void *data;
} Dict_node;

typedef struct Dict {
    Dict_node *head;
    Dict_node *tail;
    int size;
} Dict;

Dict *Dict_new();

void* Dict_set(Dict *d, char *key, void *data);
//Set a value in the dictionary, if the key already exists, the value is replaced and the old value is returned,
//if the key does not exist, the value is added and NULL is returned

void Dict_remove(Dict *d, char *key);

void* Dict_get(Dict *d, char *key);

void Dict_free(Dict *d);

void Dict_clear(Dict *d);

int Dict_Lenght(Dict *d);

void* Dict_popFront(Dict *d);

int Dict_hasKey(Dict *d, char *key);

#endif // DICTIONARY_H