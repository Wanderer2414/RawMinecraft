#include "number_extended.h"

void reverse(size_t& num) {
    size_t tmp = 0;
    while (num) {
        tmp = tmp*10 + num%10;
        num /= 10;
    }
    num = tmp;
}