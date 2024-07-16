#include "Sector.h"
#include <cmath>
// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    // TODO: Calculate the distance to the Earth, and generate the sector code
    distance_from_earth = sqrt(x*x + y*y + z*z);
    
    std::string dist_code = std::to_string(int(distance_from_earth));
    std::string x_code = (x == 0) ? "S" : (x > 0) ? "R" : "L";
    std::string y_code = (y == 0) ? "S" : (y > 0) ? "U" : "D";
    std::string z_code = (z == 0) ? "S" : (z > 0) ? "F" : "B";

    sector_code = dist_code + x_code + y_code + z_code;
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}