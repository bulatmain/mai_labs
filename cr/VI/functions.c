#include "functions.h"

void cut_last(char* s, uint64_t size) {
    char* s_ = (char*)malloc(sizeof(char) * (size - 1));
    for (uint64_t i = 0; i < size - 1; ++i) {
        s_[i] = s[i];
    }
    free(s);
    s = s_;
}

void copy_str(char* s, const char* s_, uint64_t sz1, uint64_t sz2) {
    uint64_t m = (sz1 < sz2 ? sz1 : sz2);
    for (uint64_t i = 0; i < m; ++i) {
        s[i] = s_[i];
    }
}