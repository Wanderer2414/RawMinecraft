#include "number_extended.h"

void reverse(std::size_t& num) {
    std::size_t tmp = 0;
    while (num) {
        tmp = tmp*10 + num%10;
        num /= 10;
    }
    num = tmp;
}