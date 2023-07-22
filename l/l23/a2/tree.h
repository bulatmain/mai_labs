#ifndef M_TREE_H

#define M_TREE_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    N_LEFT,
    N_RIGHT,
    N_ROOT,
    INVALID
} SIDE;

// Inary tree node structure
typedef struct node {
    // Identification number of each node
    uint64_t id;
    // Data of node
    int64_t data;
    // Pointer to left node of b_three
    struct node* left;
    // Pointer to right node of b_tree
    struct node* right;
    // Pointer to node's ancestor
    struct node* p;
} node;

// Binary tree
typedef struct b_tree{
    // Count of nodes
    int64_t n;
    // Id of last added node
    int64_t last_id;
    // Pointer to root node of b_tree   
    node* root;

} b_tree;

// Creates empty b_tree
b_tree create_empty_b_tree();

void copy_b_tree(b_tree* t, b_tree* const t_);

void copy_b_tree_n_update(b_tree* t, b_tree* const t_);

void __copy_b_subtree(node** root, node* const to_copy, node* const acc);

void init_b_tree(b_tree* t, int64_t data);

// Checks if root pointer is NULL
int8_t is_empty_b_tree(b_tree* const t);

// Adds node py pointer to its ancestor by ancestors id
void add_node_by_node(b_tree* t, node* p, int64_t data, SIDE s);

// Adds node py pointer to its ancestor by ancestors id
void add_node_by_id(b_tree* t, int64_t id, int64_t data, SIDE s);

//      Deletes node by id
void delete_node_by_id(b_tree* t, int64_t id);

//      Deletes node by pointer
void delete_node_by_node(b_tree* t, node* v);

// Returns pointer to node of b_tree by id
node* node_by_id(node* const root, int64_t id);

// Returns id node data
int64_t data_by_id(b_tree* const t, int64_t id);

// Checks if given node is in b_tree
int8_t node_is_in_b_tree(node* const root, node* const v);

// Removes all nodes from b_tree and sets it empty
void clear_tree(b_tree* t);

// Node
// {
//     id = id;
//     data = data;
//     left = NULL;
//     right = NULL;
// }
node* new_node(int64_t id, int64_t data, node* const p);

void copy_node(node* v, node* const v_);

void print_b_tree(node* const t, uint8_t tab, SIDE side);

int8_t is_B_tree(node* const root);

#define log_exception(M, ...)                                             \
{                                                                         \
    fprintf(stderr, "(%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
}

#define exception_exit(M, ...)           \
{                                        \
    log_exception(M, ##__VA_ARGS__);     \
    exit(-1);                            \
}

#endif