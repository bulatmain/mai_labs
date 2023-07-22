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
    CALC_FUNC,   // calc
    EXIT,        // exit
    PASS,        // pass or Enter
    HISTORY      // history
} COMMAND;

string* split(string* const s, string* const sep) {
    string* list = (string*)malloc(10 * sizeof(string));
    for (int8_t i = 0; i < 10; ++i) {
        list[i] = not_allocated_string();
    }
    string s_; construct_empty(&s_);
    char ch;
    uint8_t size = 0; 
    int8_t skip = 0;
    for (uint64_t i = 0; i < s->last_char; ++i) {
        ch = *at(s, i);
        for (uint64_t j = 0; j < sep->last_char; ++j) {
            if (ch == *at(sep, j)) {
                construct_from_s(&list[size], s_);
                ++size;
                destruct(&s_);
                construct_empty(&s_);
                skip = 1;
                break;
            }
        }
        if (skip) {
            skip = 0;
            continue;
        }
        add_char(&s_, ch);
    }
    return list;
}

void clear_split(string* list) {
    for (uint64_t i = 0; i < 10; ++i) {
        if (list[i].s == NULL)
            break;
        destruct(&list[i]);
    }
}

int main(int64_t argc, char** argv) {

    string history, ll, sep; 
    construct_empty(&history); construct_empty(&ll); construct_empty(&sep);

    add_char(&sep, ' ');
    add_char(&sep, '\n');
    
    printf("Please, input command:\n");
    int64_t line = 1;

    b_tree t = create_empty_b_tree();
    char* pEnd;

    while (1) {
        printf("%ld: ", line);
        construct_empty(&ll);
        read_line(&ll);
        if (ll.last_char == 0) {
            destruct(&ll);
            continue;
        }
        add_string(&history, ll);
        string* list = split(&ll, &sep);
        if        (equal_charp(&list[0], "add")) {
            if (equal_charp(&list[1], "root")) {
                int64_t val = atoll(list[2].s);
                init_b_tree(&t, val);
            } else if (equal_charp(&list[1], "node")) {
                if (is_empty_b_tree(&t)) {
                    printf("Tree is empty. \nAdd root with command: add root <value>\n");
                } else {
                    int64_t id = atoll(list[2].s);
                    int64_t val = atoll(list[3].s);
                    SIDE s;
                    if (equal_charp(&list[4], "left"))
                        s = N_LEFT;
                    else if (equal_charp(&list[4], "right"))
                        s = N_RIGHT;
                    if (node_by_id(t.root, id) == NULL) {
                        printf("There is no node in tree with id %ld\n", id);
                    } else {
                        add_node_by_id(&t, id, val, s);
                    } 
                }
                
            } else {
                printf("Wrong syntax\n");
            }
        } else if (equal_charp(&list[0], "delete")) {
            int64_t id = atoll(list[1].s);
            delete_node_by_id(&t, id);
        } else if (equal_charp(&list[0], "print")) {
            print_b_tree(t.root, 0, N_ROOT);
        } else if (equal_charp(&list[0], "calc")) {
            printf("This tree is%s%c", (is_B_tree(&t) ? " a B-tree" : " not a B-tree"), '\n');
        } else if (equal_charp(&list[0], "exit")) {
            break;
        } else if (equal_charp(&list[0], "history")) {
            print(history);
        } else {
            printf("Wrong command\n");
        }
        clear_split(list);
        destruct(&ll);
       ++line;
    }

    destruct(&history);
    destruct(&ll);
    destruct(&sep);

    clear_tree(&t);

    return 0;
}