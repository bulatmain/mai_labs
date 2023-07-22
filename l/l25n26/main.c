#include <stdio.h>
#include <inttypes.h>

#include "deque.h"


int main() {


    deque d;

    deque_create_empy(&d);

    for (size_t i = 0; i < 10; ++i) {
        deque_push_back(&d, i * i);
    }

    printf("Push back illustration:\n");
    deque_print(&d, int64_t_to_str);
    printf("\n");

    
    deque_insert(&d, 5, -10);

    printf("Insert illustration:\n");
    deque_print(&d, int64_t_to_str);
    printf("\n");


    printf("Pop back illustration:\n");
    for (size_t i = d.size; i > 0; --i) {
        deque_pop_back(&d);
        deque_print(&d, int64_t_to_str);
        printf("\n");
    }

    const size_t sz = 5;

    type a[sz];

    for (size_t i = 0; i < sz; ++i) {
        a[i] = i + 1;
    }

    deque_free(&d);

    deque_construct(&d, a, sz);

    printf("Construct from array illustration:\n");
    deque_print(&d, int64_t_to_str);
    printf("\n");

    deque_fill(&d, 9);

    printf("Fill deque illustration:\n");
    deque_print(&d, int64_t_to_str);
    printf("\n");

    d.p[3] = 8;

    printf("Erase element from deque illustration:\n");
    deque_print(&d, int64_t_to_str);
    printf("\n");

    deque_erase(&d, 3);


    deque_print(&d, int64_t_to_str);
    printf("\n");

    deque r;

    deque_create(&r, sz);

    for (size_t i = 0; i < sz; ++i) {
        deque_push_back(&r, sz - i);
    }

    deque _d;
    deque_create_fill(&_d, sz, 8);

    deque c = deque_concatenate(&d, &_d);

    printf("Concatenate deques illustration:\n");
    deque_print(&c, int64_t_to_str);
    printf("\n");

    printf("Hoar\'s sort illustration:\n");
    deque_print(&r, int64_t_to_str);
    printf("\n");

    deque f = hoars_qsort_conc(r, int64_t_less, int64_t_nless);

    deque_print(&f, int64_t_to_str);
    printf("\n");


    return 0;
}