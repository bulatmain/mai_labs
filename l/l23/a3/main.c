#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "tree.h"

typedef enum COMMAND {
    ADD_NODE,    // add node $id$ $value$ $side$ or add root $value$
    DELETE_NODE, // delete $id$
    PRINT_TREE,  // print
    // Calculating function checks if tree is a B-tree
    IS_B_TREE,   // calc
    EXIT,        // exit
    UNKNOWN      // unknown command
} COMMAND;

void print_menu() {
    printf("Menu\n");
    printf("1. Add node\n");
    printf("2. Delete node\n");
    printf("3. Print tree\n");
    printf("4. Check if three is B-tree\n");
    printf("5. Exit\n\n");
    printf("Please, choose command: ");
}

COMMAND interpret_command(int8_t c) {
    if (c == 1) {
        return ADD_NODE;
    } else if (c == 2) {
        return DELETE_NODE;
    } else if (c == 3) {
        return PRINT_TREE;
    } else if (c == 4) {
        return IS_B_TREE;
    } else if (c == 5) {
        return EXIT;
    } else {
        return UNKNOWN;
    }
}

SIDE interpret_side(char c) {
    if (c == 'L' || c == 'l') {
        return N_LEFT;
    } else if (c == 'R' || c == 'r') {
        return N_RIGHT;
    } else {
        return INVALID;
    }
}

int main(int64_t argc, char** argv) {
    
    int64_t line = 1;

    b_tree t = create_empty_b_tree();
    char* pEnd;

    COMMAND c; int32_t in, exit = 0;

    while (! exit) {

        print_menu();

        scanf("%d", &in);
        c = interpret_command(in);

        if (c == ADD_NODE) {
            if (is_empty_b_tree(&t)) {
                printf("Tree is empty now. Please enter value of root node: ");
                int64_t val; scanf("%ld", &val);
                init_b_tree(&t, val);
            } else {
                printf("Enter id of ancestor node: ");
                int64_t id; scanf("%ld", &id);
                printf("... and value of new node: ");
                int64_t val; scanf("%ld", &val);
                printf("... and side. Left or right? [L/r] ");
                SIDE s; char s_;
                scanf(" %c", &s_);
                s = interpret_side(s_);
                if (node_by_id(t.root, id) == NULL) {
                    printf("There is no node in tree with id %ld\n", id);
                } else {
                    add_node_by_id(&t, id, val, s);
                } 
            }
        } else if (c == DELETE_NODE) {
            printf("Enter id of node to be deleted: ");
            int64_t id; scanf("%ld", &id);
            delete_node_by_id(&t, id);
        } else if (c == PRINT_TREE) {
            print_b_tree(t.root, 0, N_ROOT);
        } else if (c == IS_B_TREE) {
            printf("This tree is%s%c", (is_B_tree(t.root) ? " a B-tree" : " not a B-tree"), '\n');   
        } else if (c == EXIT) {
            exit = 1;
        } else {
            printf("Wrong command.\n");
        }

        printf("-------------------------------------------\n");
    }

    clear_tree(&t);

    return 0;
}