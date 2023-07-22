#ifndef __EXPR_TREE_H

#define __EXPR_TREE_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Operations

typedef enum {
    // unary minus and plus
    _EXPR_UN_MINUS,
    _EXPR_UN_PLUS,
    // binary minus and plus
    _EXPR_BIN_MINUS,
    _EXPR_BIN_PLUS,
    // binary multiplication and division
    _EXPR_BIN_MULT,
    _EXPR_BIN_DIV,
    // binary power
    _EXPR_BIN_POW,
    // case if node is not an operation
    _EXPR_NOP
} _EXPR_OPERATION;

// Type of expression tree node
typedef enum {
    _EXPR_VAR,
    _EXPR_CONST,
    _EXPR_OP,
    _NONTYPE
} _EXPR_EL_TYPE;

typedef union {
    double num;
    char  var;
    char  op;

} expr_node_data;

// Node of expression tree
typedef struct expr_node {
    _EXPR_EL_TYPE type;
    _EXPR_OPERATION op;
    expr_node_data data;
    struct expr_node* left;
    struct expr_node* right;
    struct expr_node* ancestor;
    // Priority depth of the operation (for brackets)
    // if node is an operation
    uint8_t pd;
} expr_node;

typedef struct expr_line {
    expr_node* p1;
    expr_node* p2;
} expr_line;

// Expression tree
typedef struct expr_tree {
    // count of nodes in tree
    int64_t n;
    // tree root
    expr_node* root;
} expr_tree;

int8_t is_valid_expression(char* const e, uint32_t _size);

// Return expr_node_data variable with memory filled with 0 
expr_node_data expr_node_data_dv();

void construct_empty_expr_tree(expr_tree* t);

void construct_empty_expr_node(expr_node* p);

void init_empty_expr_node(expr_node** p, 
                          _EXPR_EL_TYPE type,
                          _EXPR_OPERATION op,
                          expr_node* const ancestor);

void parse_expr(char* const expr, uint32_t _size, expr_tree* t);

void free_expr_tree(expr_node* root);

void print_expr_tree(expr_node* root, int8_t bl, int8_t br);

void print_tree(expr_node* root, uint8_t tab);

void simplify_bin_operation(expr_node* root, _EXPR_OPERATION op);

void const_fwrd_by_op_in_line(expr_node** p1, expr_node** p2, _EXPR_OPERATION op);

expr_node* const_fwrd_by_operation(expr_node* root, _EXPR_OPERATION op);

expr_node* simplify_com_operation(expr_node* root, _EXPR_OPERATION op);

#endif