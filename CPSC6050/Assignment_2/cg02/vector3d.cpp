#include "vector3d.hpp"
#include <cmath>

namespace qunt {
vector3d::vector3d() : vector3d(0.0, 0.0, 0.0) {}

vector3d::vector3d(float x, float y) : vector3d(x, y, 0.0) {}

vector3d::vector3d(float x, float y, float z)
    : _x(x),
      _y(y),
      _z(z),
      _magnitude((float)std::sqrt(_x * _x + _y * _y + _z * _z)) {}

float vector3d::x() const { return _x; }

float vector3d::y() const { return _y; }

float vector3d::z() const { return _z; }

float vector3d::magnitude() const { return _magnitude; }
}  // namespace qunt
