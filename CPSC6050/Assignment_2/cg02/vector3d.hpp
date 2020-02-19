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
  float magnitude() const;
};
}  // namespace qunt