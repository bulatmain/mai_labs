#include "deque.h"

void deque_create_empy(deque* d) {
    d->p = (type*)calloc(1, sizeof(type));
    d->size = 0;
    d->memo = 1;
    d->begin = d->back = d->end = NULL;
} 

void deque_create(deque* d, size_t l) {
    if (l == 0) {
        deque_create_empy(d);
    }
    d->p = (type*)calloc(l, sizeof(type));
    d->size = 0;
    d->memo = l;
    d->begin = d->back = d->end = NULL;
}

void deque_create_fill(deque* d, size_t l, type f) {
    if (l == 0) {
        deque_create_empy(d);
    }
    d->p = (type*)malloc(sizeof(type) * l);
    for (size_t i = 0; i < l; ++i) {
        d->p[i] = f;
    }
    d->size = l;
    d->memo = l;
    d->begin = d->p;
    d->back = d->begin + d->size - 1;
    d->end = d->back + 1;
}

void deque_construct(deque* d, type* a, size_t l) {
    d->p = (type*)calloc(l, sizeof(type));
    d->size = l;
    d->memo = d->size;
    for (size_t i = 0; i < l; ++i) {
        d->p[i] = a[i];
    }
    d->begin = d->p;
    d->back = d->begin + d->size - 1;
    d->end = d->back + 1;
}

void deque_copy(deque* d, deque* const o) {
    d->p = (type*)calloc(o->memo, sizeof(type));
    d->size = o->size;
    d->memo = o->memo;
    for (size_t i = 0; i < o->size; ++i) {
        d->p[i] = o->p[i];
    }
    d->begin = d->p;
    d->back = d->begin + d->size - 1;
    d->end = d->back + 1;
}

bool deque_is_empty(deque* const d) {
    return d->begin == NULL && d->back == NULL;
}

bool deque_is_allocated(deque* const d) {
    return d->p == NULL;
}

void deque_clear(deque* d) {
    deque_fill(d, (type)(0));
    d->begin = d->back = d->end = NULL;
    d->size = 0;
}

void deque_free(deque* d) {
    if (!deque_is_empty(d)) {
        free(d->p);
    }
    d->p = d->begin = d->back = d->end = NULL;
    d->size = d->memo = 0;
}

void deque_push_front(deque* d, type x) {
    type* _p = d->p;
    bool allocated = false;
    if (d->size >= d->memo) {
        d->memo *= 2;
        _p = (type*)calloc(d->memo, sizeof(type));
        allocated = true;
    }
    for (size_t i = 0; i < d->size; ++i) {
        _p[i + 1] = d->p[i];
    }
    _p[0] = x;
    if (allocated) {
        free(d->p);
    }
    d->p = _p;
    d->size++;
    d->begin = d->p;
    d->back = d->p + d->size - 1;
    d->end = d->back + 1;
}

void deque_push_back(deque* d, type x) {
    type* _p = d->p;
    bool allocated = false;
    if (d->size >= d->memo) {
        d->memo *= 2;
        _p = (type*)calloc(d->memo, sizeof(type));
        allocated = true;
    }
    for (size_t i = 0; i < d->size; ++i) {
        _p[i] = d->p[i];
    }
    _p[d->size] = x;
    if (allocated) {
        free(d->p);
    }
    d->p = _p;
    d->size++;
    d->begin = d->p;
    d->back = d->p + d->size - 1;
    d->end = d->back + 1;
}

type deque_pop_front(deque* d) {
    type x = (type)(0);
    if (d->size > 0) {
        x = d->p[0];
        for (size_t i = 0; i < d->size - 1; ++i) {
            d->p[i] = d->p[i + 1];
        }
        *(d->back) = (type)(0);
    } 
    if (d->size > 1) {
        d->back--;
        d->end--;
        d->size--;
    } else {
        deque_clear(d);
    }
    return x;
}

type deque_pop_back(deque* d) {
    type x = (type)(0);
    if (d->size > 0) {
        x = *(d->back);
        *(d->back) = (type)(0);
    } 
    if (d->size > 1) {
        d->back--;
        d->end--;
        d->size--;
    } else {
        deque_clear(d);
    }
    return x;
}

void deque_fill(deque* d, type x) {
    for (size_t i = 0; i < d->size; ++i) {
        d->p[i] = x;
    }
}

void deque_print(deque* d, char* (*to_str)(type)) {
    printf("{");
    if (d->size > 0) {
        char* p;
        for (size_t i = 0; i < d->size - 1; ++i) {
            p = to_str(d->p[i]);
            printf(" %s,", p);
            free(p);
        }
        p = to_str(*(d->back));
        printf(" %s ", p);
        free(p);
    }
    printf("}");
}

size_t deque_size(deque* const d) {
    return d->size;
}

type* deque_begin(deque* const d) {
    return d->begin;
}

type* deque_back(deque* const d) {
    return d->back;
}

type* deque_end(deque* const d) {
    return d->end;
}

bool deque_insert(deque* d, size_t index, type x) {
    if (d->size < index + 1) {
        fprintf(stderr, "%s", "Error: trying to insert element out of deque bounds!\n");
        return false;
    }
    if (d->size < d->memo) {
        for (size_t i = d->size; i > index; --i) {
            d->p[i] = d->p[i - 1];
        }
        d->p[index] = x;
    } else {
        d->memo *= 2;
        type* _p = (type*)calloc(d->memo, sizeof(type));
        for (size_t i = 0; i < d->size; ++i) {
            if (i < index) {
                _p[i] = d->p[i];
            } else {
                _p[i + 1] = d->p[i];
            }
        }
        _p[index] = x;
        free(d->p);
        d->p = _p;
    }
        d->size++;
    d->begin = d->p;
    d->back = d->begin + d->size - 1;
    d->end = d->back + 1;
    return true;
}

bool deque_erase(deque* d, size_t index) {
    if (d->size < index + 1) {
        fprintf(stderr, "%s", "Error: trying to erase element out of deque bounds!\n");
        return false;
    } else {
        if (d->size > 0) {
            for (size_t i = index; i < d->size - 1; ++i) {
                d->p[i] = d->p[i + 1];
            }
            *(d->back) = (type)(0);
        }
        if (d->size > 1) {
            d->back--;
            d->end--;
            d->size--;
        } else {
            deque_clear(d);
        }
    }
    return true;
}

deque deque_filter(deque* d, type x, bool (*cmp)(type, type)) {
    deque r;
    deque_create_empy(&r);
    for (size_t i = 0; i < d->size; ++i) {
        if (cmp(d->p[i], x)) {
            deque_push_back(&r, d->p[i]);
        }
    }
    return r;
}

deque deque_concatenate(deque* const a, deque* const b) {
    deque d;
    deque_create_empy(&d);
    d.p = (type*)calloc(a->size + b->size, sizeof(type));
    d.size = a->size + b->size;
    d.memo = d.size;

    for (size_t i = 0; i < a->size; ++i) {
        d.p[i] = a->p[i];
    }
    for (size_t i = a->size; i < d.size; ++i) {
        d.p[i] = b->p[i - a->size];
    }
    d.begin = d.p;
    d.back = d.begin + d.size - 1;
    d.end = d.back + 1;
    return d;
}

void hoars_qsort_iter(type* begin, type* end, bool (*cmp)(type, type)) {
    if (end - begin > 1) {
        // Pick pivot
        type* back = end - 1;
        type* mid = begin + (back - begin) / 2;
        if (cmp(*mid, *begin)) {
            swap(*mid, *begin, type);
        }
        if (cmp(*back, *begin)) {
            swap(*back, *begin, type);
        }
        if (cmp(*back, *mid)) {
            swap(*back, *mid, type);
        }
        type pivot = *mid;
        // Hoar's partition scheme
        type* i = begin,* j = back;
        while (true) {
            while (j - i > 0 && cmp(*i, pivot)) {
                ++i;
            }
            while (j - i > 0 && cmp(pivot, *j)) {
                --j;
            }
            if (i - j >= 0) {
                break;
            }
            swap(*i, *j, type);
            ++i; --j;
        }
        hoars_qsort_iter(begin, j, cmp);
        hoars_qsort_iter(j + 1, end, cmp);
    }
}

deque hoars_qsort_conc(deque d, bool (*less)(type, type), bool(*nless)(type, type)) {
    if (d.size <= 1) {
        return d;
    }
    type pivot = average(*d.begin, *(d.begin + (d.back - d.begin) / 2), *d.back, less);
    deque l = hoars_qsort_conc(deque_filter(&d, pivot, less), less, nless);
    deque r = hoars_qsort_conc(deque_filter(&d, pivot, nless), less, nless);
    deque res = deque_concatenate(&l, &r);
    deque_free(&l); deque_free(&r);
    return res;
}

type average(type a, type b, type c, bool (*less)(type, type)) {
    if (less(a, b)) {
        if (less(b, c)){
            return b;
        } else {
            return c;
        }
    } else if (less(a, c)) {
        return a;
    } else if (less(b, c)) {
        return c;
    } else {
        return b;
    }
}

char* int64_t_to_str(int64_t x) {
    char* p;
    if (x == 0) {
        p = (char*)malloc(sizeof(char) * 2);
        p[0] = '0';
        p[1] = '\0';
    }
    else {
        int64_t _x = x;
        size_t l = 0;
        while (_x != 0) {
            _x /= 10;
            ++l;
        }
        if (x < 0) {
            p = (char*)malloc(sizeof(char) * (l + 2));
            p[0] = '-';
            ++l;
            x = -x;
        } else {
            p = (char*)malloc(sizeof(char) * (l + 1));
        }
        p[l] = '\0';
        while (x != 0) {
            p[--l] = (char)((x % 10) + 48);
            x /= 10;
        }
    }
    
    return p;
}

bool int64_t_less(int64_t a, int64_t b) {
    return a < b;
}

bool int64_t_nmore(int64_t a, int64_t b) {
    return a <= b;
}

bool int64_t_more(int64_t a, int64_t b) {
    return a > b;
}

bool int64_t_nless(int64_t a, int64_t b) {
    return a >= b;
}

bool int64_t_eq(int64_t a, int64_t b) {
    return a == b;
}

bool int64_t_neq(int64_t a, int64_t b) {
    return a != b;
}

