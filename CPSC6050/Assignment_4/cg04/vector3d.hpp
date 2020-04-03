#pragma once

namespace qunt {
class vector3d {
 private:
  float _x;
  float _y;
  float _z;
  float _magnitude;

 public:
  vector3d();
  vector3d(float x, float y);
  vector3d(float x, float y, float z);

  float x() const;
  float y() const;
  float z() const;

  void set_x(float x);
  void set_y(float y);
  void set_z(float z);

  void set(float x, float y);
  void set(float x, float y, float z);

  float magnitude() const;

 private:
  float calculate_magnitude() const;
  void set_magnitude();
};
}  // namespace qunt