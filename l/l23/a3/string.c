#include "string.h"

// Процедура конструирования строки из одного символа ' '
void construct_empty(string* s) {
    construct_from_n(s, 1);
}

string not_allocated_string() {
	string s;
	annul(&s);
	return s;
}

// Процедура конструирования строки из n символов
void construct_from_n(string* s, uint64_t n) {
    // Один последний символ зарезервирован под '\0'
	s->s = (char*)malloc(sizeof(char) * n + 1);
	if (!(s->s == NULL)) {
		s->memory_size = n;
		s->last_char = 0;
        // Строка инициализирована символом INIT_VALUE, объявленном в string.h
		for (uint64_t i = 0; i < n; ++i)
			s->s[i] = INIT_VALUE;
        s->s[n] = '\0';
	}
    else {
        log_info("REFUZE_MEMORY_ALLOCATION\n");
        exit(-1);
    }
}

// Процедура конструирования строки из другой строки
void construct_from_s(string* s, string s_) {
	construct_from_n(s, s_.memory_size);
	copy_string(s, s_);
}

// Процедура конструирования строки из массива символов
void construct_from_char_pointer(string* s, const char* p) {
	if (p == NULL) {
        log_info("NULL_POINTER_ACCES\n");
        exit(-1);
    }
	construct_from_n(s, 1);
	uint64_t i = 0;
	while (p[i] != '\0') {
		add_char(s, p[i]);
		++i;
	}
    add_char(s, '\0');
}

// Процедура деструктирования строки
void destruct(string* s) {
	if (s->s) {
		free(s->s);
		s->last_char = 0;
		s->memory_size = 0;
	}
    else {
	    log_info("DEALLOCATE_NULL_POINTER");
        exit(-1);
    }
}

// Аннулировать строку 
void annul(string* s) {
	s->s = NULL;
	s->last_char = 0;
	s->memory_size = 0;
}

// Процедура присвоения памяти
void appropriate_memory(string* s, string* s_) {
	destruct(s);
	s->s = s_->s;
	s->memory_size = s_->memory_size;
	s->last_char = s_->last_char;
	annul(s_);
}

// Процедура копирования строки (без учёта памяти)

// Пример:
// <'abc', '1234'> -> <'123', '1234'>
// <'abcd', '123'> -> <'123 ', '123'>
// <'abc',  '123'> -> <'123',  '123'>
void copy_string(string* s, string s_) {
	if (s->s && s_.s) {
		uint64_t i = 0;
		while (i < s->memory_size && i < s_.last_char) {
			s->s[i] = s_.s[i];
			++i;
		}
		while (i < s->memory_size) {
			s->s[i] = ' ';
			++i;
		}
        s->last_char = min(s->memory_size, s_.last_char);
	}
	else {
        log_info("NULL_POINTER_ACCES");
        exit(-1);
    }
}

// Процедура присвоения строки
void appropriate_string(string* s, string s_) {
	construct_from_s(s, s_);
	copy_string(s, s_);
}


// Процедура добавления памяти в строку
void add_memory(string* s, uint64_t n) {
	string s_;
	construct_from_n(&s_, s->memory_size * n);
	copy_string(&s_, *s);
	appropriate_memory(s, &s_);
}

// Добавить символ
void add_char(string* s, char c) {
	if (s->s) {
		if (s->last_char >= s->memory_size) {
			add_memory(s, ADD_MEMORY_COEFFICIENT);
		}
		s->s[s->last_char] = c;
        if (c != '\0')
            s->last_char++;
		s->s[s->last_char] = '\0';
	}
	else {
        log_info("NULL_POINTER_ACCES");
        exit(-1);
    }
}

// Конкатенация строк
void add_string(string* s, string s_) {
    string S;
    construct_from_n(&S, s->last_char + s_.last_char);
    while (S.last_char < s->last_char) {
        S.s[S.last_char] = s->s[S.last_char];
        S.last_char++;
    }
    while (S.last_char - s->last_char < s_.last_char) {
        S.s[S.last_char] = s_.s[S.last_char - s->last_char];
        S.last_char++;
    }
    add_char(&S, '\0');
    appropriate_memory(s, &S);
}

int8_t equal_string(string* const s, string* const s_) {
	if (s->last_char != s_->last_char) {
		return 0;
	}
	for (uint64_t i = 0; i < s->last_char; ++i) {
		if (*at(s, i) != *at(s_, i))
			return 0;
	}
	return 1;
}

int8_t equal_charp(string* const s, const char* p) {
	string s_;
	construct_from_char_pointer(&s_, p);
	return equal_string(s, &s_);
}

// Считать строку
int8_t read_line(string* s) {
	char c;
	while (1) {
		//scanf("%c", &c);
		c = getchar();
        if (c == EOF || c == '~') {
            add_char(s, '\0');
            return 0; // Конец потока
        }
        if (c == '\n') {
			add_char(s, c);
            return 1; // Конец строки
        }
		add_char(s, c);
	}
}

// Считать все строки
void read(string* s) {
    while (read_line(s)) {}
}

// Вывести строку
void print(string s) {
	printf("%s", s.s);
}

// Получить i-тый символ строки
char* at(string* s, uint64_t i) {
	if (i < s->memory_size) {
		return &s->s[i];
    }
    else {
	    log_info("OCCUPIED_MEMORY_ACCES");
        exit(-1);
    }
}