#include "../Project.hpp"
bool tryFree(void *ptr) {
    try {
        free(ptr);
    }
    catch (std::exception ex) {
        return false;
    }
    return true;
}
