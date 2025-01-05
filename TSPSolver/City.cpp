#include "City.h"

City::City() {
    index = -1;
    x = -1;
    y = -1;
}

City::City(int index, float x, float y) : index(index), x(x), y(y) {

}

City& City::operator=(const City& other) {
    if (this != &other) {
        index = other.index;  // Copy other member variables as needed
        x = other.x;
        y = other.y;
        // Additional assignment if needed
    }
    return *this;
}

bool City::is_Empty() {
    return index == -1;
}
