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
