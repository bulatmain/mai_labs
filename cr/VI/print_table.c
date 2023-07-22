#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cell.h"
#include "functions.h"

void print_object(const cell* s, uint64_t ind) {
    printf("%ld\t|", ind);
    uint8_t j = 0;
    while (s->NAME[j] != '\0') {
        printf("%c", s->NAME[j]); ++j;
    }   
    while (j < 20) {
        printf(" "); ++j;
    }
    if (s->sex) {
        printf("|female |");
    } else {
        printf("|  male |");
    }
    j = 0;
    while (s->group[j] != '\0') {
        printf("%c", s->group[j]); ++j;
    }   
    while (j < 15) {
        printf(" "); ++j;
    }
    printf("|  %d  | %d  |   %d   |   %d    |\n",\
           s->marks[0].mark, s->marks[1].mark, s->marks[2].mark, s->marks[3].mark);

}

void print_help() {
    printf("This programm prints objects of given table.\n");
    printf("Parameters:\n");
    printf("\t-t or --tablename\tset name of table\n");
    printf("\t-c or --count    \tset count of objects to be printed. if not specified prints all objects depending on regime\n");
    printf("\t-p or --parameter\tset group that only female students of group have only one 5 mark\n");
    printf("\t-f or --footprint\tstandart printing, set as default\n");
    printf("\t-h or --help      \tshow this help page\n");
}

uint8_t count_of_marks(const cell* s, uint8_t mark) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MARKS_COUNT; ++i)
        if (s->marks[i].mark == mark) 
            ++count;
    return count;
}

// int8_t cmp_groups(char g1[GROUP_SIZE], char g2[GROUP_SIZE]) {
//     uint8_t i = 0;
//     while (g1[i] == g2[i]) {
//         ++i;
//     }
//     if (0 < i && (g1[i] == '\0' || g2[i] == '\0'))
//
// }

int main(uint8_t argc, char** argv) {

    char* tablename = (char*)malloc(sizeof(char) * TABLENAME_LENGTH);
    char group[GROUP_SIZE];
    uint64_t count = 0;
    int8_t name_given = 0, count_given = 0;


    int8_t regime = -1;

    for (uint8_t k = 1; k < argc; ++k) {
        if        (! (strcmp(argv[k], "-t") && strcmp(argv[k], "--tablename"))) {
            strcpy(tablename, argv[k + 1]); name_given = 1; ++k;
        } else if (! (strcmp(argv[k], "-c") && strcmp(argv[k], "--count"))) {
            char* end;
            count = strtol(argv[k + 1], &end, 10); count_given = 1; ++k;
            if (*end) {
                char* arg = (char*)malloc(sizeof(char) * 20);
                strcpy(arg, argv[k + 1]);
                printf("Wrong count was given: %s\n", arg);
                free(arg);
                free(tablename);
                exit(0);
            }
        } else if (! (strcmp(argv[k], "-p") && strcmp(argv[k], "--parameter"))) {
            if (regime != -1) {
                printf("Please choose only one regime of printng\n");
                free(tablename);
                exit(-1);
            }
            regime = 1;
            strcpy(group, argv[k + 1]); ++k;
        }  else if (! (strcmp(argv[k], "-f") && strcmp(argv[k], "--footprint"))) {
            if (regime != -1) {
                printf("Please choose only one regime of printng\n");
                free(tablename);
                exit(-1);
            }
            regime = 0;
        } else if (! (strcmp(argv[k], "-h") && strcmp(argv[k], "--help"))) {
            print_help();
            free(tablename);
            exit(0);
        } else {
            char* arg = (char*)malloc(sizeof(char) * 20);
            strcpy(arg, argv[k]);
            printf("Wrong parameter: %s\n", arg);
            free(arg);
            free(tablename);
            exit(-1);
        }
    }



    if (! name_given) {
        printf("No name given, please input name of list that is supposed to be printed\n");
        free(tablename);
        exit(-1);
    }

    if (! count_given) {
        count = UINT64_MAX;
    }

    if (count == 0) {
        free(tablename);
        exit(0);
    }

    FILE* f = fopen(tablename, "r");

    if (f == NULL) {
        printf("Can not open file\n");
        exit(-1);
    }

    if (regime == -1) {
        regime = 0;
    }

    cell student;
    
    uint64_t i = 1;
    while (1) {
        if (i > count) {
            break;
        }
        if (fread(&student, sizeof(student), 1, f) == 1) {
        } else {
            if (feof(f)) {
                //printf("End of file\n");
                free(tablename);
                exit(0);
            } else {
                printf("Error while reading file\n");
                exit(-1);
            }
        }
    
        if (i == 1) {
            printf("#\t|        NAME        |  Sex  |     Group     |"                      \
                        " ADS | DM | LinAl | MathAn |\n");
        }
        if (regime == 1) {
            if (student.sex == 1 && count_of_marks(&student, 5) == 1 && ! (strcmp(student.group, group)))
                print_object(&student, i);
        } else {
            print_object(&student, i);
        }
        ++i;
    }

    free(tablename);

    return 0;
}