#include "expr_tree.h"

int8_t is_operation(char c) {
    switch (c)
    {
    case '-':
        return 1;
    case '+':
        return 1;
    case '*':
        return 1;
    case '/':
        return 1;
    case '^':
        return 1;
    default:
        return 0;
    }
}

int8_t is_commutative(_EXPR_OPERATION op) {
    switch (op)
    {
    case _EXPR_BIN_DIV:
        return 1;
    case _EXPR_BIN_MULT:
        return 1;
    case _EXPR_BIN_MINUS:
        return 1;
    case _EXPR_BIN_PLUS:
        return 1;
    default:
        return 0;
    }
}

_EXPR_OPERATION operation(char c, int8_t unary) {
    if (c == '-' && unary) {
        return _EXPR_UN_MINUS;
    } else if (c == '+' && unary) {
        return _EXPR_UN_PLUS;
    } else if (c == '-') {
        return _EXPR_BIN_MINUS;
    } else if (c == '+') {
        return _EXPR_BIN_PLUS;
    } else if (c == '*') {
        return _EXPR_BIN_MULT;
    } else if (c == '/') {
        return _EXPR_BIN_DIV;
    } else if (c == '^') {
        return _EXPR_BIN_POW;
    } else {
        return _EXPR_NOP;
    }
}

// priority of operations
int8_t expr_op_prty(_EXPR_OPERATION op) {
    switch (op)
    {
    case _EXPR_BIN_MINUS:
        return 1;
    case _EXPR_BIN_PLUS:
        return 1;
    case _EXPR_BIN_MULT:
        return 2;
    case _EXPR_BIN_DIV:
        return 2;
    case _EXPR_UN_MINUS:
        return 3;
    case _EXPR_UN_PLUS:
        return 3;
    case _EXPR_BIN_POW:
        return 4;    
    case _EXPR_NOP:
        return 0;
    default:
        return -1;
    }
}

int8_t is_valid_expression(char* const _e, uint32_t _size) {
    char* p = _e;
    if (p == NULL) {
        return 0;
    }

    char* _p = (char*)malloc(sizeof(char) * _size);
    p = _p;
    uint32_t size = 0;
    for (uint32_t i = 0; i < _size; ++i) {
        if (_e[i] != ' ') {
            p[size++] = _e[i];
        }
    }

    int8_t res = 1;

    // Is first letter, count of parentheses not closed 
    int8_t fl = 1, psc = 0;
    while (*p != '\0') {
        // There should be no other operation and after could be only digit, parenthesis or variable
        if (is_operation(*p)) {
            // Incorrect what's after
            if (*(p + 1) == '\0' ||
                !(isdigit(*(p + 1)) || *(p + 1) == '(' || isalpha(*(p + 1)))) {
                res = 0; break;
            }
            // Incorrect what's before if
                // unary
            if (fl && !(*p == '-' || *p == '+')) {
                res = 0; break;
            }
                // binary
            if (!(*p == '-' || *p == '+') && (*(p - 1) == '(' || is_operation(*(p - 1)))) {
                res = 0; break;
            }
        // Variable or first and has operation after or last and has operation before
        // or has operation before and after or single
        } else if (isalpha(*p)) {
            if (! (fl || is_operation(*(p - 1)) || *(p - 1) == '(')){
                res = 0; break;
            }
            if (! (fl || ((*(p + 1) == '\0') || is_operation(*(p + 1)) || *(p + 1) == ')'))) {
                res = 0; break;
            }
        // Number is just as variable but it could be longer than one symbol
        } else if (isdigit(*p)) {
            if (! (fl || is_operation(*(p - 1)) || *(p - 1) == '(')) {
                res = 0; break;
            }
            int8_t count_of_dots = 0;
            while (*p != '\0' && (isdigit(*p) || *p == '.')) {
                if (*p == '.') {
                    ++count_of_dots;
                    if (1 < count_of_dots) {
                        res = 0; break;
                    }
                }
                ++p;
            }
            if (!res) {
                break;
            }
            --p;
            if (! ((*(p + 1) == '\0') || is_operation(*(p + 1)) || *(p + 1) == ')')) {
                res = 0; break;
            }
        // Open bracket should has operation before it if not first symbol
        // and should not be last and should not has another open bracket after it
        } else if (*p == '(') {
            if (! (fl || is_operation(*(p - 1)) || *(p - 1) == '(')) {
                res = 0; break;
            }
            if ((*(p + 1) == '\0') || *(p + 1) == ')') {
                res = 0; break;
            }
            ++psc;
        // Close bracket should not be first, has operation or open bracker before it and should be last
        // or has operation after it
        } else if (*p == ')') {
            if (fl || is_operation(*(p - 1)) || *(p - 1) == '(') {
                res = 0; break;
            }
            if (! (*(p + 1) == '\0' || is_operation(*(p + 1)) || *(p + 1) == ')')) {
                res = 0; break;
            }
            --psc;
        } else {
            res = 0; break;
        }
        fl = 0;
        ++p;
    }

    free(_p);

    return res && psc == 0;
}

expr_node_data expr_node_data_dv() {
    expr_node_data _;
    _.num = 0;
    return _;
}

void construct_empty_expr_tree(expr_tree* t) {
    t->n = 0;
    t->root = NULL;
}

void construct_empty_expr_node(expr_node* p) {
    init_empty_expr_node(&p, _NONTYPE, _EXPR_NOP, NULL);
}

void init_empty_expr_node(expr_node** p, 
                          _EXPR_EL_TYPE type,
                          _EXPR_OPERATION op,
                          expr_node* const ancestor) {
    *p = (expr_node*)malloc(sizeof(expr_node));
    (*p)->type = type;
    (*p)->op = op;
    (*p)->ancestor = ancestor;
    (*p)->data.var = 0;
    (*p)->pd = 0;
    (*p)->left = NULL;
    (*p)->right = NULL;
}

double expr_make_num(char** _c) {
    char** c = _c;
    double f = 0, i = 10;
    int8_t phase = 1;
    while (**c != '\0' && (isdigit(**c) || **c == '.')) {
        if (**c == '.') {
            phase = 2;
            ++(*c);
        }
        if (phase == 1) {
            f = 10 * f + (**c - 48);
        } else if (phase == 2) {
            f += (double)(**c - 48) / i;
            i *= 10;
        }
        ++(*c);
    }
    --(*c);

    return f;
}

void parse_expr(char* const _expr, uint32_t _size, expr_tree* t) {
    char* expr = (char*)malloc(sizeof(char) * _size);
    uint32_t size = 0;
    for (uint32_t i = 0; i < _size; ++i) {
        if (_expr[i] != ' ') {
            expr[size++] = _expr[i];
        }
        if (_expr[i] == '\0') {
            break;
        }
    }
      
    construct_empty_expr_tree(t);

    expr_node* p = NULL;

    int8_t empty = 1;

    // Priority depth. Each time parenthesis opens priority depth 
    // of next operations increases and decreases when closes 
    int8_t pd = 0;

    // Priority of last operation
    int8_t lp = 0;

    char* c = expr,* pr = NULL;
    while (*c != '\0') {
        // Case if read symbol was a letter means that we're reading a variable
        if (isalpha(*c)) {
            // If tree is empty we should create root
            if (empty) {
                init_empty_expr_node(&p, _EXPR_VAR, _EXPR_NOP, NULL);
                p->data.var = *c;
                t->root = p;
                empty = 0;
            } else {
                // Variable or constant can be read before operation only in the beggining
                init_empty_expr_node(&p->right, _EXPR_VAR, _EXPR_NOP, p);
                p = p->right;
                p->data.var = *c;
            }
        // Case if readen symbol is '-', '+', '*', '/' or '^' means that we're reading an operation
        } else if (is_operation(*c)) {
            // Unary
            if (p == NULL || p->type == _EXPR_OP) {
                if (p == NULL) {
                    init_empty_expr_node(&p, _EXPR_OP, operation(*c, 1), NULL);
                    p->data.op = *c;
                    p->pd = pd;
                    t->root = p;
                    empty = 0;
                    lp = expr_op_prty(operation(*c, 1)) + pd;
                } else {
                    init_empty_expr_node(&p->right, _EXPR_OP, operation(*c, 1), p);
                    p = p->right;
                    p->data.op = *c;
                    p->pd = pd;
                    lp = expr_op_prty(operation(*c, 1)) + pd;
                }
            // Binary
            } else {
                if (expr_op_prty(operation(*c, 0)) + pd < lp) {
                    while (! (p->ancestor == NULL) && 
                           expr_op_prty(operation(*c, 0)) + pd < expr_op_prty(p->ancestor->op) + p->ancestor->pd) {
                        p = p->ancestor;
                    }
                    expr_node* _p = p->ancestor;
                    init_empty_expr_node(&p->ancestor, _EXPR_OP, operation(*c, 0), NULL);
                    p->ancestor->left = p;
                    p = p->ancestor;
                    if (! (_p == NULL)) {
                        _p->right = p;
                        p->ancestor = _p;
                    }
                    p->data.op = *c;
                    p->pd = pd;
                    if (p->ancestor == NULL) {
                        t->root = p;
                    }
                    lp = expr_op_prty(operation(*c, 0)) + pd;
                } else {
                    expr_node* _p;
                    init_empty_expr_node(&_p, _EXPR_OP, operation(*c, 0), p->ancestor);
                    _p->data.op = *c;
                    _p->pd = pd;
                    if (! (p->ancestor == NULL)) {
                        p->ancestor->right  =_p;
                    }
                    _p->left = p;
                    p->ancestor = _p;
                    p = _p;
                    if (p->ancestor == NULL) {
                        t->root = p;
                    }
                    lp = expr_op_prty(operation(*c, 0)) + pd;
                }
            }
        // Case if read sybmol was a bracket
        } else if (*c == '(') {
            pd += 10;
        } else if (*c == ')') {
            pd -= 10;
        // Cse if read symbol was a digit
        } else if (isdigit(*c)) {
            // If tree is empty we should create root
            if (empty) {
                init_empty_expr_node(&p, _EXPR_CONST, _EXPR_NOP, NULL);
                p->data.num = expr_make_num(&c);
                t->root = p;
                empty = 0;
            } else {
                // Variable or constant can be read before operation only in the beggining
                init_empty_expr_node(&p->right, _EXPR_CONST, _EXPR_NOP, p);
                p = p->right;
                p->data.num = expr_make_num(&c);
            }
        }
        ++c;
    }
}

int8_t expr_op_is_unary(_EXPR_OPERATION op) {
    return op == _EXPR_UN_PLUS || op == _EXPR_UN_MINUS;
}

void print_expr_tree(expr_node* root, int8_t bl, int8_t br) {
    if (root == NULL) {
        return;
    }
    if (root->type == _EXPR_OP) {
        if (! (root->ancestor == NULL) && 
            root->ancestor->pd < root->pd) {
            print_expr_tree(root->left, bl + 1, 0);
        } else {
            print_expr_tree(root->left, bl, 0);
        }
    }
    if (root->type == _EXPR_OP) {
        if (expr_op_is_unary(root->op)) {
            for (int8_t i = 0; i < bl + 1; ++i) {
                printf("(");
            }
            printf("%c", root->data.op);
        } else {
            printf(" %c ", root->data.op);
        }
    } else if (root->type == _EXPR_VAR) {
        for (int8_t i = 0; i < bl; ++i) {
            printf("(");
        }
        printf("%c", root->data.var);
        for (int8_t i = 0; i < br; ++i) {
            printf(")");
        }
    } else if (root->type == _EXPR_CONST) {
        for (int8_t i = 0; i < bl; ++i) {
            printf("(");
        }
        if ((root->data.num - (int)root->data.num) < 1e-2) {
            printf("%.d", (int)root->data.num);
        } else {
            printf("%.2f", root->data.num);
        }
        for (int8_t i = 0; i < br; ++i) {
            printf(")");
        }
    }
    if (root->type == _EXPR_OP) {
        if ((! (root->ancestor == NULL) && 
            root->ancestor->pd < root->pd) ||
            expr_op_is_unary(root->op)) {
            print_expr_tree(root->right, 0, br + 1);
        } else {
            print_expr_tree(root->right, 0, br);
        }
    }
}

void free_expr_tree(expr_node* root) {
    if (root == NULL) {
        return;
    }
    free_expr_tree(root->left);
    free_expr_tree(root->right);
    (*root).left = NULL;
    (*root).right = NULL;
    free(root);
}

void make_operation(expr_node* p1, expr_node *p2, _EXPR_OPERATION op) {
    switch (op)
    {
    case _EXPR_BIN_MULT:
        p1->data.num *= p2->data.num;
        break;
    case _EXPR_BIN_DIV:
        if (p1->data.num == 0) {
            printf("Division by zero error: %.4f / %.4f\n", 
            p1->data.num, p2->data.num);
            exit(-1);
        }
        p1->data.num /= p2->data.num;
        break;
    case _EXPR_BIN_PLUS:
        p1->data.num += p2->data.num;
        break;
    case _EXPR_BIN_MINUS:
        p1->data.num += p2->data.num;
        break;
    case _EXPR_NOP:
        break; 
    default:
        printf("Wrong operation: %d\n", op);
        break;
    }
}

void simplify_simple_operation(expr_node* p1, expr_node *p2, _EXPR_OPERATION op) {
    if (! (p2 == NULL)) {
        while (1) {
            make_operation(p1->left, p1->right->left, op);
            expr_node* _p = p1->right;
            p1->right = p1->right->right;
            p1->right->ancestor = p1;
            if (_p == p2) {
                free(_p);
                break;
            } else {
                free(_p);
            }
        }
    }
    
    if (p1->right->type == _EXPR_CONST) {
        p1->type = _EXPR_CONST;
        p1->op = _EXPR_NOP;
        p1->pd = 0;
        make_operation(p1->left, p1->right, op);
        p1->data.num = p1->left->data.num;
        free(p1->left);
        free(p1->right);
        p1->left = NULL;
        p1->right = NULL;
    }
}

void simplify_bin_operation(expr_node* root, _EXPR_OPERATION op, expr_node* p1, expr_node* p2) {
    if (root == NULL) {
        return;
    }
    // If node is an operation
    if (root->type == _EXPR_OP) {
        simplify_bin_operation(root->left, op, NULL, NULL);
        // If is simple operation
        if (root->op == op && root->left->type == _EXPR_CONST) {
            if (p1 == NULL) {
                simplify_bin_operation(root->right, op, root, NULL);
            } else {
                simplify_bin_operation(root->right, op, p1, root);
            }
        } else if (! (p2 == NULL)) {
            simplify_simple_operation(p1, p2, op);
            simplify_bin_operation(root->right, op, NULL, NULL);
        } else {
            simplify_bin_operation(root->right, op, p1, NULL);
        }
    } else if (! (p1 == NULL)) {
        simplify_simple_operation(p1, p2, op);
    }
}

// Move constants forward in expression like a1 ∘ a2 ∘ a3 ∘ ... ∘ an, where some of a1,...,an are constants
// Should be used only for commutatuive operations
void const_fwrd_by_op_in_line(expr_node** p1, expr_node** p2, _EXPR_OPERATION op) {

    expr_node* p = *p1;

    while (1) {
        const_fwrd_by_operation(p->left, op, NULL, NULL);
        if (p == *p2) {
            break;
        }
        p = p->right;
    }

    expr_node* begin = *p1;
    p = begin->right;
    
    while (p->type == _EXPR_OP) {
        if (p == *p2) {
            *p2 = (*p2)->right;
        }
        if (p->left->type == _EXPR_CONST && p->pd == p->ancestor->pd) {
            p->right->ancestor = p->ancestor;
            p->ancestor->right = p->right;

            if (begin->left->type == _EXPR_CONST) {
                begin->right->ancestor = p;
                p->right = begin->right;
                begin->right = p;
                p->ancestor = begin;
            } else {
                p->ancestor = begin->ancestor;
                if (! (begin->ancestor == NULL)) {
                    if (begin->ancestor->right == begin) {
                        begin->ancestor->right = p;
                    } else {
                        begin->ancestor->left = p;                    
                    }
                }                
                begin->ancestor = p;
                p->right = begin;
                begin = p;
            }
        }
        if (! ((*p2)->type == _EXPR_OP)) {
            *p2 = (*p2)->ancestor;
        }
        p = p->right;
    }
    p = p->ancestor;
    if (p->right->type == _EXPR_CONST && p->pd == p->ancestor->pd) {    
        *p2 = (*p2)->ancestor;    
        p->ancestor->right = p->left;
        p->left->ancestor = p->ancestor;

        p->left = p->right;
        p->right = begin->left;
        begin->left->ancestor = p;

        begin->left = p;
        p->ancestor = begin;
    }
    *p1 = begin;
}

expr_line _const_fwrd_by_operation(expr_node* root, _EXPR_OPERATION op, expr_node* p1, expr_node* p2) {
    if (root == NULL) {
        return (expr_line){ .p1 = NULL, .p2 = NULL };
    }
    if (root->type == _EXPR_OP) {
        if (root->op == op) {
            expr_line l;
            if (! (p1 == NULL)) {
                l = _const_fwrd_by_operation(root->right, op, p1, root);
            } else {
                l = _const_fwrd_by_operation(root->right, op, root, NULL);            
            }
            p1 = l.p1, p2 = l.p2;
            if (! (p1 == NULL)) {
                if (p2 == NULL) {
                    _const_fwrd_by_operation(p1->left, op, NULL, NULL);
                    _const_fwrd_by_operation(p1->right, op, NULL, NULL);
                } else if (p2->right->type == _EXPR_OP) {
                    _const_fwrd_by_operation(p2->right, op, NULL, NULL);
                }
            }        
        } else {
            _const_fwrd_by_operation(root->left, op, NULL, NULL);
            _const_fwrd_by_operation(root->right, op, NULL, NULL);
        }
    } else if (! (p2 == NULL)) {
        const_fwrd_by_op_in_line(&p1, &p2, op);
        return (expr_line){ .p1 = p1, .p2 = p2 };
    }
    return (expr_line){ .p1 = NULL, .p2 = NULL };
}

expr_node* back_to_root(expr_node* p) {
    if (! (p->ancestor == NULL)) {
        p = p->ancestor;
    } else {
        return p;
    }
    return back_to_root(p);
}

expr_node* const_fwrd_by_operation(expr_node* root, _EXPR_OPERATION op, expr_node* p1, expr_node* p2) {
    _const_fwrd_by_operation(root, op, NULL, NULL);
    return back_to_root(root);
}

// Simplify expression by commutative operation
expr_node* simplify_com_operation(expr_node* root, _EXPR_OPERATION op) {
    root = const_fwrd_by_operation(root, op, NULL, NULL);
    simplify_bin_operation(root, op, NULL, NULL);
    return root;
}