#include "revolution.hpp"

#include <cmath>

bool lessThanX(const qunt::point3d& p1, const qunt::point3d& p2) {
  return p1.y() < p2.y();
}

int factorial(int number) {
  static std::vector<int> values{1, 1};
  if (number >= (int)values.size()) {
    int value = factorial(number - 1) * number;
    values.push_back(value);
  }

  return values[number];
}

std::vector<qunt::point3d> getQuinticBezier(std::vector<qunt::point3d> points,
                                            int width, int height) {
  std::sort(points.begin(), points.end(), lessThanX);
  for (qunt::point3d& point : points) {
    point.set_x(point.x() * width);
    point.set_y(point.y() * height);
  }

  float distance = 0;
  int n = 5;
  for (int i = 1; i <= n; i++) {
    distance += (points[i] - points[i - 1]).magnitude();
  }

  std::vector<int> coefficients(n + 1);
  int nFact = factorial(n);
  for (int i = 0; i <= n; i++) {
    coefficients[i] = nFact / factorial(i) / factorial(n - i);
  }

  float dt = 1.0 / distance;
  std::vector<qunt::point3d> returnPoints;
  for (float t = 0; t < 1.0; t += dt) {
    float x = 0, y = 0;
    for (int i = 0; i <= n; i++) {
      float constVal =
          coefficients[i] * std::pow((1 - t), n - i) * std::pow(t, i);
      x += (constVal * points[i].x());
      y += (constVal * points[i].y());
    }
    returnPoints.emplace_back(x / width, y / height);
  }

  return returnPoints;
}