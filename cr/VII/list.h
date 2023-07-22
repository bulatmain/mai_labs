#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint64_t size_t;

typedef struct {
    char* p;
    size_t l;
} string;

string make_string(char* const);
bool strings_eq(string, string);

typedef struct node {
    struct node* r;
    double x;
} node;

node* make_node(double);

void link_node(node*, node*, node*);

void link_nodes(node*, node*);

void node_free(node*);

typedef struct {
    node* begin;
    node* back;
    size_t size;
} list;

void list_create_empty(list*);

list list_copy(list* const);

void list_add(list*, double, size_t);

node* list_find_node(list* const, double);

void list_remove(list*, double);

void list_remove_subtree(list*, node*, node*);

void list_free(list*);

size_t list_len(node* const, node* const);

void list_print(list* const);

void list_cut(list*, char, double);


#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : (-x))

#endif