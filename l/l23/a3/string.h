#ifndef STRING_H

#define STRING_H

#define ADD_MEMORY_COEFFICIENT 2
#define INIT_VALUE ' '

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// Структура Строка
struct string {
	char* s;
	uint64_t memory_size;
	uint64_t last_char;
};

typedef struct string string;

// Процедура конструирования строки из одного символа INIT_VALUE
void construct_empty(string* s);

string not_allocated_string();

// Процедура конструирования строки из n символов
void construct_from_n(string* s, uint64_t n);

// Процедура конструирования строки из другой строки
void construct_from_s(string* s, string s_);

// Процедура конструирования строки из массива символов
void construct_from_char_pointer(string* s, const char* p);

// Процедура деструктирования строки
void destruct(string* s);

// Аннулировать строку 
void annul(string* s);

// Процедура присвоения памяти
void appropriate_memory(string* s, string* s_);

// Процедура присвоения строки
void appropriate_string(string* s, string s_);

// Процедура копирования строки (без учёта памяти)

// Пример: <'abc'; '1234'> -> <'123'; '1234'>
// Пример: <'abcd'; '123'> -> <'123 '; '123'>
void copy_string(string* s, string s_);

// Процедура добавления памяти в строку
void add_memory(string* s, uint64_t n);

// Процедура добавления символа в строку
void add_char(string* s, char c);

// Конкатенация строк
void add_string(string* s, string s_);

int8_t equal_string(string* const s, string* const s_);

int8_t equal_charp(string* const s, const char* p);

// Считать строку
int8_t read_line(string* s);

// Считать все строки
void read(string* s);

// Вывести строку
void print(string s);

// Получить i-тый символ строки
char* at(string* s, uint64_t i);

// Макросы

// Максимум
#define max(a, b) (a > b ? a : b)
// Минимум
#define min(a, b) (a < b ? a : b)
// Логирование
#define log_info(M, ...) fprintf(stderr, "(%s:%d) " M "\n",\
        __FILE__, __LINE__, ##__VA_ARGS__)

#endif