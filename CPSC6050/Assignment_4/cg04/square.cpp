#include "square.hpp"

square::square(const std::vector<glm::vec4>& points) : points(points) {}

square::square(const glm::vec4 &p1, const glm::vec4 &p2, const glm::vec4 &p3,
         const glm::vec4 &p4) : points{p1, p2, p3, p4} {}

glm::vec4& square::operator[](int i) {
  return const_cast<glm::vec4&>((*this)[i]);
}

const glm::vec4& square::operator[](int i) const {
  if (i == 0) {
    return points[0];
  } else if (i == 1 || i == 3) {
    return points[1];
  } else if (i == 2 || i == 4) {
    return points[2];
  } else {
    return points[3];
  }
}

void square::scale(float mult) {
  for (auto& point : points) {
    point *= mult;
  }
}

void square::translate(const glm::vec4 dist) {
  for (auto& point : points) {
    point += dist;
  }
}