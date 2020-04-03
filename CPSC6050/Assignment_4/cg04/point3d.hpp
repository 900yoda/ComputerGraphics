#pragma once

#include "vector3d.hpp"
#include <iostream>

namespace qunt {
class point3d {
 private:
  float _x;
  float _y;
  float _z;

 public:
  point3d();
  point3d(float x, float y);
  point3d(float x, float y, float z);

  float x() const;
  float y() const;
  float z() const;

  void set_x(float x);
  void set_y(float y);
  void set_z(float z);

  void set(float x, float y);
  void set(float x, float y, float z);

  vector3d operator-(const point3d &p) const;
  point3d operator+(const vector3d &vec) const;

 private:
  friend point3d get_mid_point(const point3d &p1, const point3d &p2);
  friend std::ostream& operator<<(std::ostream& stream, const point3d &point);
};

point3d get_mid_point(const point3d &p1, const point3d &p2);

std::ostream& operator<<(std::ostream& stream, const point3d &point);

}  // namespace qunt