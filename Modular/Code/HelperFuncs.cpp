#include "../Project.hpp"
namespace helpers {
bool tryFree(void *ptr) {
    try {
        free(ptr);
    }
    catch (std::exception ex) {
        return false;
    }
    return true;
}


}