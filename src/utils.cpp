#include "../headers/utilities.h"

void usage(const char* name, const char* arguments) {
    std::cout << "Usage: " << name << ' ' << arguments << std::endl;
    exit(64);
}

int long2a(unsigned long number, char** str) {
    int length = 0;
    unsigned long tmp = number;
    while (tmp > 0) {
        tmp /= 10;
        length++;
    }
    (*str) = new char[length + 1];
    return sprintf((*str), "%lu", number);
}

unsigned long a2long(char* str) {
    char* endptr;
    return strtoul(str, &endptr, 0);
}
