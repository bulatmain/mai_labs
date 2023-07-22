#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "list.h"


typedef struct {
    list l;
    double mxe;
    size_t ind;
} sparce_matrix;

void read_sparse_matrix_from_file(sparce_matrix* mx, const char* fname) {
    FILE* f = fopen(fname, "r");
    if (f == NULL) {
        printf("Unable to open file %s", fname);
        exit(-1);
        return;
    }

    mx->mxe = 0;
    list* l = &(mx->l);
    list_create_empty(l);
    list_add(l, 0, l->size);
    list_add(l, 1, l->size);
    char c = '~';
    int64_t i = 1, j = 0;

    double x = 0;

    size_t fraction = 0;
    int8_t multiplier = 1;

    int64_t last_column = -1, penult_column = -1;

    while (true) {
        c = fgetc(f);
        
        if (c == ' ') {
            multiplier = 1;
            fraction = 0;
            list_add(l, j + 1, l->size);
            list_add(l, x, l->size);
            x = 0;
            ++j;
        } else if (c == '\n') {
            multiplier = 1;
            fraction = 0;
            list_add(l, j + 1, l->size);
            list_add(l, x, l->size);
            list_add(l, 0, l->size);
            list_add(l, i + 1, l->size);
            x = 0;
            ++i; j = 0;
        } else if (c == EOF) {
            list_add(l, j + 1, l->size);
            list_add(l, x, l->size);
            break;
        } else if (isdigit(c)) {
            if (fraction) {
                x += (double)(c - 48) / fraction * multiplier;
                fraction *= 10;
            } else {
                x = 10 * x + (c - 48) * multiplier;
            }
        } else if (c == '.') {
            fraction = 10;
        }  else if (c == '-') {
            multiplier = -1;
        }
        if (abs(mx->mxe) < abs(x)) {
            last_column = j;
            penult_column = -1;
            mx->mxe = x;
            mx->ind = j;
        } else if (abs(abs(mx->mxe) - abs(x)) < 1e-7) {
            if (last_column < j) {
                if (-1 < penult_column) {
                    mx->mxe = x;
                    mx->ind = last_column;
                }
                penult_column = last_column;
                last_column = j;
            } else if (penult_column < j && j < last_column) {
                penult_column = j;
                mx->mxe = x;
                mx->ind = penult_column;
            }
        }
    }

    list_add(l, 0, l->size);
    list_add(l, 0, l->size);

    fclose(f);
}

void print_sparse_matrix(list* l) {
    node* p = l->begin;
    if (p == NULL) {
        return;
    }
    int64_t j = 0;
    while (true) {
        if (p->x == 0) {
            if (! (p == l->begin)) {
                printf("\n"); 
            }
            j = 0;
            if (p->r->x == 0) {
                break;
            }
            p = p->r;
        } else {
            while (j + 1 < (int64_t)(p->x)) {
                printf("0\t");
                ++j;
            }
            p = p->r; ++j;
            if (abs(p->x - (int64_t)(p->x)) < 1e-3) {
                printf(" %ld\t", (int64_t)(p->x));
            } else {
                printf(" %.3f\t", p->x);
            }
        }
        p = p->r;
    }
}

sparce_matrix calc_func(sparce_matrix mx) {
    sparce_matrix _mx;
    _mx.l = list_copy(&(mx.l));
    _mx.mxe = mx.mxe;
    _mx.ind = mx.ind;

    node* p = _mx.l.begin;
    while (true) {
        if (p->x == 0) {
            if (p->r->x == 0) {
                break;
            }
            p = p->r;
        } else {
            if (p->x == _mx.ind + 1) {
                p = p->r;
                p->x /= _mx.mxe;
            } else {
                p = p->r;
            }
        }
        p = p->r;
    }
    return _mx;
}

int main(int arc, char** argv) {

    sparce_matrix mx;

    for (int i = 1; i < arc; ++i) {
        printf("File: %s\n\n", argv[i]);

        read_sparse_matrix_from_file(&mx, argv[i]);

        printf("Origin matrix:\n");
        print_sparse_matrix(&(mx.l));
        printf("Internal representation:\n");
        list_print(&(mx.l));

        printf("\n" );

        sparce_matrix _mx = calc_func(mx);

        printf("Calculated matrix:\n");
        print_sparse_matrix(&(_mx.l));
        printf("Internal representation:\n");
        list_print(&(_mx.l));
        printf("\n");

        list_free(&(mx.l));
        list_free(&(_mx.l));
        printf("\n\n");
    }

    


    return 0;
}