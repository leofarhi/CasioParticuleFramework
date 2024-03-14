#include "Dict.h"

Dict *Dict_new() {
    Dict *d = malloc(sizeof(Dict));
    d->head = NULL;
    d->tail = NULL;
    d->size = 0;
    return d;
}

//Set a value in the dictionary, if the key already exists, the value is replaced and the old value is returned,
//if the key does not exist, the value is added and NULL is returned
void* Dict_set(Dict *d, char *key, void *data) {
    Dict_node *node = d->head;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            void *old = node->data;
            node->data = data;
            return old;
        }
        node = node->next;
    }
    node = malloc(sizeof(Dict_node));
    node->key = malloc(strlen(key) + 1);
    strcpy(node->key, key);
    node->data = data;
    node->next = NULL;
    node->prev = d->tail;
    if (d->tail != NULL) {
        d->tail->next = node;
    }
    d->tail = node;
    if (d->head == NULL) {
        d->head = node;
    }
    d->size++;
    return NULL;
}

void Dict_remove(Dict *d, char *key) {
    Dict_node *node = d->head;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
            if (node == d->head) {
                d->head = node->next;
            }
            if (node == d->tail) {
                d->tail = node->prev;
            }
            free(node->key);
            free(node);
            d->size--;
            return;
        }
        node = node->next;
    }
}

void* Dict_get(Dict *d, char *key) {
    Dict_node *node = d->head;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}

void Dict_free(Dict *d)
{
    Dict_clear(d);
    free(d);
}

void Dict_clear(Dict *d)
{
    Dict_node *node = d->head;
    while (node != NULL) {
        Dict_node *next = node->next;
        free(node->key);
        free(node);
        node = next;
    }
    d->head = NULL;
    d->tail = NULL;
    d->size = 0;
}

int Dict_Lenght(Dict *d)
{
    return d->size;
}

void* Dict_popFront(Dict *d)
{
    if (d->head == NULL)
        return NULL;
    Dict_node *node = d->head;
    void *data = node->data;
    d->head = node->next;
    if (d->head != NULL)
        d->head->prev = NULL;
    else
        d->tail = NULL;
    free(node->key);
    free(node);
    d->size--;
    return data;
}

int Dict_hasKey(Dict *d, char *key)
{
    Dict_node *node = d->head;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}