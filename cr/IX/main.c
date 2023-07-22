#define TABLE_IMPLEMENTATION
#include "table.h"


int main() {
    table t = table_alloc();

    uint64_t count = 0;
    scanf("%ld", &count);
    
    printf("Table: ");
    for (uint64_t i = 0; i < count; ++i) {

        char *key_s = (char*) calloc(256, sizeof(char));
        scanf("%s", key_s);

        int key_int = 0;
        scanf("%d", &key_int);

        char *value = (char*) calloc(256, sizeof(char));
        scanf("%s", value);

        table_push(&t, key_s, key_int, value);

        free(key_s);
    }
    printf("\n");

    table_print(t);
    printf("\n");
    table s = counting_sort(&t);
    table_print(s);

    int search = 1;
    while (search) {
        char *key_s = (char*) calloc(256, sizeof(char));
        int key_int = 0;
        scanf("%s", key_s);
        scanf("%d", &key_int);
        printf("Search for:(%s, %d)\n", key_s, key_int);

        printf("Found string: %s\n", table_binary_search(s, key_s, key_int));
        free(key_s);

        printf("Do you want to continue the search? (0/1): ");
        scanf("%d", &search);
    }

    printf("\n");

    table_dealloc(&t);
}