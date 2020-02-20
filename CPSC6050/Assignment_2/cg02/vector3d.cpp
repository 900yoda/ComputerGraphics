#include "vector3d.hpp"
#include <cmath>

namespace qunt {
vector3d::vector3d() : vector3d(0.0, 0.0, 0.0) {}

vector3d::vector3d(float x, float y) : vector3d(x, y, 0.0) {}

vector3d::vector3d(float x, float y, float z)
    : _x(x), _y(y), _z(z), _magnitude(calculate_magnitude()) {}

float vector3d::x() const { return _x; }

float vector3d::y() const { return _y; }

float vector3d::z() const { return _z; }

void vector3d::set_x(float x) {
  _x = x;
  set_magnitude();
}

void vector3d::set_y(float y) {
  _y = y;
  set_magnitude();
}

void vector3d::set_z(float z) {
  _z = z;
  set_magnitude();
}

void vector3d::set(float x, float y) {
  _x = x;
  _y = y;
  set_magnitude();
}

void vector3d::set(float x, float y, float z) {
  _z = z;
  set(x, y);
  set_magnitude();
}

float vector3d::magnitude() const { return _magnitude; }

float vector3d::calculate_magnitude() const {
  return (float)std::sqrt(_x * _x + _y * _y + _z * _z);
}

void vector3d::set_magnitude() { _magnitude = calculate_magnitude(); }

}  // namespace qunt
