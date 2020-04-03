#pragma once

#include <vector>

#include "glm/glm.hpp"

class square {
 private:
  std::vector<glm::vec4> points;

 public:
  square(const std::vector<glm::vec4> &points);
  square(const glm::vec4 &p1, const glm::vec4 &p2, const glm::vec4 &p3,
         const glm::vec4 &p4);
  glm::vec4 &operator[](int i);
  const glm::vec4 &operator[](int i) const;
  void scale(float mult);
  void translate(const glm::vec4 dist);
};