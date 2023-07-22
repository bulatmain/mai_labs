#ifndef _CELL_H

#define _CELL_H

#include <stdlib.h>
#include <inttypes.h>

#define NAME_SIZE 20
#define GROUP_SIZE 15
#define MARKS_COUNT 4

#define TABLENAME_LENGTH 20

typedef struct mark {
    int8_t type; // Зачет/экзамен - 0/1 
    int8_t mark;  // Оценка (от 2 до 5, если экзамен и 0 или 1, если зачет)

} mark;

typedef struct cell {
    char NAME[NAME_SIZE];     // ФИО
    int8_t sex;               // Пол
    char group[GROUP_SIZE];   // Номер группы
    mark marks[MARKS_COUNT];  // Оценки по предметам:
                              // АСД, Дискретная математика, 
                              // ЛинАл, МатАн, Практикум программирования, 
                              // Разработка IT-проектов, Физическая культура, 
                              // Филосовия, Экономическая теория, Иностранный язык

} cell;

#endif