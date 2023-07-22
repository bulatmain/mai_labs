#include <stdio.h>
#include <stdlib.h>

#include "expr_tree.h"

int main() {

    int n;
    scanf("%d", &n);

    expr_tree t;

    for (int i = 0; i < n; ++i) {
        char* s = (char*)malloc(sizeof(char) * 50);
        scanf(" %49[^\n]", s);
        if (is_valid_expression(s, 49)) {
            parse_expr(s, 49, &t);
            printf("Origin expression tree construction:\n");
            print_tree(t.root, 0);
            t.root = simplify_com_operation(t.root, _EXPR_BIN_MULT);
            printf("Expression tree after transformation:\n");
            print_tree(t.root, 0);
            printf("And result expression:\n");
            print_expr_tree(t.root, 0, 0);
            printf("\n");
            free_expr_tree(t.root);
        } else {
            printf("Expression is not valid\n");
        }
        free(s);
    }

    

    return 0;
}