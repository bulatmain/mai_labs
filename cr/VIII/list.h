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

string make_string(char*);
bool strings_eq(string, string);

typedef struct node {
    struct node* r;
    string s;
} node;

node* make_node(string);

void link_node(node*, node*, node*);

void link_nodes(node*, node*);

void node_free(node*);

typedef struct {
    node* begin;
    node* back;
    size_t size;
} list;

void list_create_empty(list*);

void list_add(list*, string, size_t);

node* list_find_node(list*, string);

void list_remove(list*, string);

void list_remove_subtree(list*, node*, node*);

void list_free(list*);

size_t list_len(node*, node*);

void list_print(list*);

void list_cut(list*, char, string);


#endif