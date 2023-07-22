#include <stdio.h>
#include <inttypes.h>

#include "list.h"

#define STRING_LENGTH 32

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

int main() {

    list l;
    
    list_create_empty(&l);

    int32_t command = 0;
    bool running = true;

    char* s;
    size_t size;


    while (running) {
        printf("Menu:\n0. Exit\n1. Add element to the list\n2. Remove element from the list\n3. Cut sides.\n4. Print list\n5. Print list length\n");
        printf("Please, input command: ");
        scanf(" %d", &command);

        switch (command)
        {
        case 1:
            s = (char*)calloc(STRING_LENGTH, sizeof(char));
            printf("Input string to be added: ");
            size = scanf(" %s[^\n]", s);
            list_add(&l, (string){.l = size, .p = s}, l.size);
            break;
        case 2:
            s = (char*)calloc(STRING_LENGTH, sizeof(char));
            printf("Input string that is to be removed from the list: ");
            size = scanf(" %s[^\n]", s);
            list_remove(&l, (string){.l = size, .p = s});
            break;
        case 3:
            s = (char*)calloc(STRING_LENGTH, sizeof(char));
            printf("Input central element: ");
            size = scanf(" %s[^\n]", s);
            printf("Which side you wish to be cut? [l, r]: ");
            char side;
            scanf(" %c[^\n]", &side);
            if (! (side == 'l' || side == 'r')) {
                printf("Wrong side picked: %c\n", side);
            } else {
                list_cut(&l, side, (string){.l = size, .p = s});
            }
            break;
        case 4:
            list_print(&l);
            printf("\n");
            break;
        case 5:
            printf("List length: %ld\n", l.size);
            break;
        case 0:
            running = false;
            break;
        default:
            printf("Wrong command picked\n");
            break;
        }
    }

    list_free(&l);

    return 0;
}