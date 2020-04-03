#include "point3d.hpp"

namespace qunt {
point3d::point3d() : point3d(0.0, 0.0, 0.0) {}

point3d::point3d(float x, float y) : point3d(x, y, 0.0) {}

point3d::point3d(float x, float y, float z) : _x(x), _y(y), _z(z) {}

float point3d::x() const { return _x; }

float point3d::y() const { return _y; }

float point3d::z() const { return _z; }

void point3d::set_x(float x) { _x = x; }

void point3d::set_y(float y) { _y = y; }

void point3d::set_z(float z) { _z = z; }

void point3d::set(float x, float y) {
  _x = x;
  _y = y;
}
void point3d::set(float x, float y, float z) {
  set(x, y);
  _z = z;
}

vector3d point3d::operator-(const point3d &p) const {
  return vector3d(_x - p._x, _y - p._y, _z - p._z);
}

point3d point3d::operator+(const vector3d &vec) const {
  return point3d(_x + vec.x(), _y + vec.y(), _z + vec.z());
}

point3d get_mid_point(const point3d &p1, const point3d &p2) {
  float x = (p1._x + p2._x) / 2;
  float y = (p1._y + p2._y) / 2;
  float z = (p1._z + p2._z) / 2;

  return point3d(x, y, z);
}

std::ostream &operator<<(std::ostream &stream, const point3d &point) {
  stream << "(" << point.x() << "," << point.y() << "," << point.z() << ")";
  return stream;
}
}  // namespace qunt
