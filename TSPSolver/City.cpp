#include "City.h"

// Constructor sets defaults to variables
City::City() {
    index = -1;
    x = -1;
    y = -1;
}

City::City(int index, float x, float y) {
    this->index = index;
    this->x = x;
    this->y = y;
}

// Assign operator efficiently assigns City
City& City::operator=(const City& other) {
    if (this != &other) {
        index = other.index;
        x = other.x;
        y = other.y;
    }
    return *this;
}

// If default index, city is 'empty' or uninitialised
bool City::is_Empty() {
    return index == -1;
}
