#ifndef _UDT_H
#define _UDT_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int64_t type;
typedef uint64_t size_t;

typedef struct {
    // Pointer to allocated memory
    type* p;
    // Pointer to the firest element of the deque
    type* begin;
    // Pointer to the last element of the deque
    type* back;
    // Pointer to place right after back
    type* end;
    // Count of elements in deque
    size_t size;
    // Count of elements allocated in memory
    size_t memo;
} deque;

// Constructs deque with diven length
void deque_create(deque*, size_t);

// Constructs deque with diven length
void deque_create_fill(deque*, size_t, type);

// Constructs empty deque
void deque_create_empy(deque*);

// Construct deque from given array
void deque_construct(deque*, type*, size_t);

// Copy deque from other given deque
void deque_copy(deque*, deque* const);

// Checks if deque is empty
bool deque_is_empty(deque* const);
// Checks if deque is allocated
bool deque_is_allocated(deque* const);

void deque_clear(deque*);

void deque_free(deque*);

// Push and pop from each side
void deque_push_front(deque*, type);
void deque_push_back(deque*, type);
type deque_pop_front(deque*);
type deque_pop_back(deque*);

void deque_fill(deque*, type);

// Print deque in console
void deque_print(deque*, char* (*)(type));

// Size of deque
size_t deque_size(deque* const);

type* deque_begin(deque* const);
type* deque_back(deque* const);
type* deque_end(deque* const);

// Insert
bool deque_insert(deque*, size_t, type);
bool deque_erase(deque*, size_t);

// Filters
deque deque_filter(deque*, type, bool (*)(type, type));

// Concatenate deques

deque deque_concatenate(deque* const, deque* const);

// With iterating 
void hoars_qsort_iter(type*, type*, bool (*)(type, type));

// With concatenating left and right sorted deqs
deque hoars_qsort_conc(deque, bool (*)(type, type), bool (*)(type, type));

#define swap(a, b, _type) \
{                         \
    _type c = a;          \
    a = b;                \
    b = c;                \
}

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

type average(type, type, type, bool (*)(type, type));

char* int64_t_to_str(int64_t);

bool int64_t_less(int64_t, int64_t);
bool int64_t_nmore(int64_t, int64_t);
bool int64_t_more(int64_t, int64_t);
bool int64_t_nless(int64_t, int64_t);
bool int64_t_eq(int64_t, int64_t);
bool int64_t_neq(int64_t, int64_t);

#endif