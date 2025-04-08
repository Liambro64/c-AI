#include "../Project.hpp"
bool tryFree(void *ptr) {
    try {
        free(ptr);
    }
    catch (std::exception e) {
        return false;
    }
    return true;
}

const void print(char *format, ...) {
    va_list lst;
    va_start(lst, format);  
    if (DEBUG())
        vprintf(format, lst);
}