#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "cell.h"
#include "functions.h"


#define close_files() {          \
    if (! (namelist == NULL)) {  \
        fclose(namelist);        \
    }                            \
    if (! (grouplist == NULL)) { \
        fclose(grouplist);       \
    }                            \
    if (! (marklist == NULL)) {  \
        fclose(marklist);        \
    }                            \
}

#define free_data() { \
    free(names);      \
    free(groups);     \
    free(marks);      \
    free(tablename);  \
}

void print_help() {
    printf("This programm generates data tables with information of studiends as their name, sex, group and marks.\n");
    printf("Parameters:\n");
    printf("\t-t or --tablename\tset name of generating table.             Default value is my_table\n");
    printf("\t-r or --regime   \tset regime of writing: rewrite or append. Default value is ab\n");
    printf("\t-n or --namelist \tset list of using names.                  Default value is stdnamelist\n");
    printf("\t-g ot --grouplist\tset list of using groups.                 Default value is stdgrouplist\n");
    printf("\t-m or --marklist \tset list of using marks.                  Default value is stdmarklist\n");
    printf("\t-c or --count    \tset count of generating objects.          If not specified - programm will create empty table\n");
    printf("\t-h or --help     \tshow this help page.\n");
    exit(0);
}

int main(int8_t argc, char** argv) {

    // Count of objects to generate
    uint64_t count = 5;
    // Pointers to data files
    FILE* namelist = NULL,* grouplist = NULL,* marklist = NULL;
    // Name of table
    char* tablename = (char*)malloc(sizeof(char) * TABLENAME_LENGTH);
    int8_t name_given = 0;
    char regime[2] = "ab"; // "w" - rewrite, "a" - augment.

    // Parse input
    for (int8_t i = 1; i < argc; ++i) {
        if        (! (strcmp(argv[i], "-t") && strcmp(argv[i], "--tablename"))) {
            strcpy(tablename, argv[i + 1]); name_given = 1; ++i;
        } else if (! (strcmp(argv[i], "-r") && strcmp(argv[i], "--regime"))) {
            strcpy(regime, argv[i + 1]); ++i;
            if ((regime[0] != 'w') && (regime[0] != 'a')) {
                printf("Wrong regime given: %s\n", regime);
                close_files();
                exit(-1);
            }

        } else if (! (strcmp(argv[i], "-n") &&  strcmp(argv[i], "--namelist"))) {
            namelist = fopen(argv[i + 1], "r"); ++i;
        } else if (! (strcmp(argv[i], "-g") &&  strcmp(argv[i], "--grouplist"))) {
            grouplist = fopen(argv[i + 1], "r"); ++i;
        } else if (! (strcmp(argv[i], "-m") && strcmp(argv[i], "--marklist"))) {
            marklist = fopen(argv[i + 1], "r"); ++i;
        } else if (! (strcmp(argv[i], "-c") &&  strcmp(argv[i], "--count"))) {
            char* end;
            count = strtol(argv[i + 1], &end, 10); ++i;
            if (*end) {
                char* arg = (char*)malloc(sizeof(char) * 20);
                strcpy(arg, argv[i + 1]);
                printf("Wrong count was given: %s\n", arg);
                free(arg);
                free(tablename);
                close_files();
                exit(-1);
            }
        } else if (! (strcmp(argv[1], "-h") && strcmp(argv[1], "--help"))) {
            print_help();
            free(tablename);
            close_files();
            exit(0);
        } else {
            char* arg = (char*)malloc(sizeof(char) * 20);
            strcpy(arg, argv[i]);
            printf("Wrong parameter: %s\n", arg);
            free(arg);
            free(tablename);
            close_files();
            exit(-1);
        }
    }

    if (! name_given) {
        strcpy(tablename, "my_table");
    }

    // Creating pointer to table file
    FILE* table = fopen(tablename, regime);

    // Check if opened
    if (table == NULL) {
        printf("Error: Can not create or open table with filename: %s\n", tablename);
        close_files();
    
        exit(-1);
    }

    // Set default data files
    if (namelist == NULL) {
        namelist = fopen("stdnamelist", "r");
        if (namelist == NULL) {
            printf("Error: Can not open the standard list of names.\n");
            free(tablename);
            close_files();
            exit(-1);
        } 
    }
    if (grouplist == NULL) {
        grouplist = fopen("stdgrouplist", "r");
        if (grouplist == NULL) {
            printf("Error: Can not open the standard list of groups.\n");
            free(tablename);
            close_files();
            exit(-1);
        } 
    }
    if (marklist == NULL) {
        marklist = fopen("stdmarklist", "r");
        if (marklist == NULL) {
            printf("Error: Can not open the standard list of marks.\n");
            free(tablename);
            close_files();
            exit(-1);
        } 
    }

    if (count == 0) {
        printf("%s table created, 0 objects was generated\n", tablename);
        free(tablename);
        close_files();
        exit(0);
    }

    // Upload data in memory
    char** names, ** groups; int8_t* marks,* sexes;
    uint32_t cnames, csexes, cgroups, cmarks;

    // Names and sexes
    {
        size_t len = 20; size_t chrs;
        char* buff = (char*)malloc(sizeof(char) * len);
        if ((chrs = getline(&buff, &len, namelist)) != -1) {
            sscanf(buff, "%d", &cnames);
            csexes = cnames;
            names = (char**)malloc(sizeof(char*) * cnames);
            sexes = (int8_t*)malloc(sizeof(int8_t) * csexes);
            uint8_t i = 0;
            while ((chrs = getline(&buff, &len, namelist)) != -1) {
                if (i % 2 == 0) {
                    names[i / 2] = (char*)malloc(sizeof(char) * len);
                    copy_str(names[i / 2], buff, chrs - 1, len);  ++i;
                } else {
                    sexes[i / 2] = buff[0] - 48; ++i;
                }
            }
            if (i % 2 != 0) {
                printf("Error: Incorrectly set namelist file: odd NAME or sex\n");
            }
            if ((i / 2) < cnames) {
                cnames = i / 2;
                csexes = i / 2;
            }
        } else {
            printf("Error: Can not read from namelist\n");
            close_files();
            exit(-1);
        }
        free(buff);
    }
    // Groups
    {
        size_t len = 12; int8_t chrs;
        char* buff = (char*)malloc(sizeof(char) * len);
        if ((chrs = getline(&buff, &len, grouplist)) != -1) {
            sscanf(buff, "%d", &cgroups);
            groups = (char**)malloc(sizeof(char*) * cgroups);
            uint8_t i = 0;
            while ((chrs = getline(&buff, &len, grouplist)) != -1) {
                groups[i] = (char*)malloc(sizeof(char) * len);
                copy_str(groups[i], buff, chrs - 1, len); ++i;
            }
            if (i < cgroups) {
                cgroups = i;
            }
        } else {
            printf("Error: Can not read from grouplist\n");
            close_files();
            exit(-1);
        }
        
    }
    // Marks
    {
        size_t len = 2; int8_t chrs;
        char* buff = (char*)malloc(sizeof(char) * len);
        if ((chrs = getline(&buff, &len, marklist)) != -1) {
            sscanf(buff, "%d", &cmarks); 
            marks = (int8_t*)malloc(sizeof(int8_t) * cmarks);
            uint8_t i = 0;
            while ((chrs = getline(&buff, &len, marklist)) != -1) {
                marks[i] = buff[0] - 48; ++i;
            }
            if (i < cmarks) {
                cmarks = i;
            }
        } else {
            printf("Error: Can not read from marklist\n");
            close_files();
            exit(-1);
        }
        
    }
    
    // Close files
    close_files();

    // List of generating objects 
    cell student;

    srand(time(NULL)); // Setting random

    size_t r;
    for (uint64_t i = 0; i < count; ++i) {
        uint64_t r = rand();
        strcpy(student.NAME, names[r % cnames]);
        student.sex = sexes[r % csexes];
        strcpy(student.group, groups[rand() % cgroups]);
        for (uint8_t j = 0; j < MARKS_COUNT; ++j) {
            student.marks[j].mark = marks[rand() % cmarks];
            if (1 < student.marks[j].mark) {
                student.marks[j].type = 0; // Test
            } else {
                student.marks[j].type = 1; // Exam
            }
        }
        fwrite(&student, sizeof(student), 1, table);
    }

    if (regime == "wb")
        printf("%s table created, %ld objects was generated\n", tablename, count);
    else if (regime == "ab")
        printf("%s table appended, %ld objects was generated\n", tablename, count);

    free_data();
    fclose(table);

    return 0;
} 