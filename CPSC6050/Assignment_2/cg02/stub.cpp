//
//		          Programming Assignment #1
//
//					Daljit Singh Dhillon
//
//
/***************************************************************************/
/* Include needed files */

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "point3d.hpp"
#include "vector3d.hpp"

/***************************************************************************/
// Forward declarations
void drawPixel(int x, int y);

// Main tasks for 4050
void drawCircle(int centerX, int centerY, int pointOnCricleX,
                int pointOnCricleY);
void drawParabola(int vertexX, int vertexY, int pointOnParabolaX,
                  int pointOnParabolaY);
void drawCubicBezier(int* ptX, int* ptY);
/***************************************************************************/
// some helper functions

float to_degrees(float radians) {
  return (int)((radians + M_PI) * 180 / M_PI) % 360;
}

int factorial(int number) {
  static std::vector<int> values{1, 1};
  if (number >= (int)values.size()) {
    int value = factorial(number - 1) * number;
    values.push_back(value);
  }

  return values[number];
}
/***************************************************************************/
// Main tasks for 6050
void drawArc(int ptX1, int ptY1, int ptX2, int ptY2, int ptX3, int ptY3);
void drawEllipse(int centerX, int centerY, int ptX1, int ptY1, int ptX2,
                 int ptY2);  // Bonus for 4050
void drawPoly(int ptX1, int ptY1, int ptX2, int ptY2);
void drawQuinticBezier(int* ptX, int* ptY);
/***************************************************************************/
// Few Bonus tasks
void drawCardinalSpline(int* ptX, int* ptY, int controlPointCount);
void drawCubicBSpline(int* ptX, int* ptY,
                      int controlPointCount);  // Not for 4050
void drawMidpointParabola(int vertexX, int vertexY, int pointOnParabolaX,
                          int pointOnParabolaY);
/***************************************************************************/

// Main tasks for 4050
/***************************************************************************/
void drawCircle(int centerX, int centerY, int pointOnCricleX,
                int pointOnCricleY) {
  drawPixel(centerX, centerY);
  drawPixel(pointOnCricleX, pointOnCricleY);
  // replace above two lines with your code
}

// Draw Parabola y-squared equals 'a' times x
// vertex is the tip of the parabola
void drawParabola(int vertexX, int vertexY, int pointOnParabolaX,
                  int pointOnParabolaY) {
  drawPixel(vertexX, vertexY);
  drawPixel(pointOnParabolaX, pointOnParabolaY);
  // replace above two lines with your code
}

void drawCubicBezier(int* ptX, int* ptY) {
  drawPixel(ptX[0], ptY[0]);
  // replace above line with your code
}

// Main tasks for 6050
/***************************************************************************/
void drawArc(int ptX1, int ptY1, int ptX2, int ptY2, int ptX3, int ptY3) {
  qunt::point3d p1(ptX1, ptY1);
  qunt::point3d p2(ptX2, ptY2);
  qunt::point3d p3(ptX3, ptY3);

  // get the two points for the line for the first bisector
  qunt::point3d m1 = qunt::get_mid_point(p1, p3);
  qunt::vector3d v1 = p3 - p1;
  v1.set(-v1.y(), v1.x());
  qunt::point3d m2 = m1 + v1;

  // get the two points for the line for the second bisector
  qunt::point3d m3 = qunt::get_mid_point(p2, p3);
  qunt::vector3d v2 = p3 - p2;
  v2.set(-v2.y(), v2.x());
  qunt::point3d m4 = m3 + v2;

  float x = ((m1.x() * m2.y() - m1.y() * m2.x()) * (m3.x() - m4.x()) -
             (m1.x() - m2.x()) * (m3.x() * m4.y() - m3.y() * m4.x())) /
            ((m1.x() - m2.x()) * (m3.y() - m4.y()) -
             (m1.y() - m2.y()) * (m3.x() - m4.x()));

  float y = ((m1.x() * m2.y() - m1.y() * m2.x()) * (m3.y() - m4.y()) -
             (m1.y() - m2.y()) * (m3.x() * m4.y() - m3.y() * m4.x())) /
            ((m1.x() - m2.x()) * (m3.y() - m4.y()) -
             (m1.y() - m2.y()) * (m3.x() - m4.x()));

  qunt::point3d center(x, y);

  qunt::vector3d line1Vec = p1 - center;
  qunt::vector3d line2Vec = p2 - center;
  qunt::vector3d line3Vec = p3 - center;

  float radius = line1Vec.magnitude();
  float p1Angle = to_degrees(std::atan2(line1Vec.x(), line1Vec.y()));
  float degreeOffSet = 360 - p1Angle;
  float p2Angle =
      (int)(to_degrees(std::atan2(line2Vec.x(), line2Vec.y())) + degreeOffSet) %
      360;
  float p3Angle =
      (int)(to_degrees(std::atan2(line3Vec.x(), line3Vec.y())) + degreeOffSet) %
      360;
  bool shouldBeInside = p2Angle <= p3Angle;

  qunt::point3d drawPoint(0, radius);
  float pk = 5.0 / 4 - radius;

  while (drawPoint.x() < drawPoint.y()) {
    std::vector<qunt::point3d> drawPoints{
        {drawPoint.x(), drawPoint.y()},   {drawPoint.y(), drawPoint.x()},
        {drawPoint.x(), -drawPoint.y()},  {drawPoint.y(), -drawPoint.x()},
        {-drawPoint.x(), -drawPoint.y()}, {-drawPoint.y(), -drawPoint.x()},
        {-drawPoint.x(), drawPoint.y()},  {-drawPoint.y(), drawPoint.x()}};

    for (unsigned i = 0; i < drawPoints.size(); i++) {
      qunt::point3d& p = drawPoints[i];
      float pointAngle =
          (int)(to_degrees(std::atan2(p.x(), p.y())) + degreeOffSet) % 360;

      if ((pointAngle < p3Angle && shouldBeInside) ||
          (pointAngle > p3Angle && !shouldBeInside)) {
        drawPixel(center.x() + p.x(), center.y() + p.y());
      }
    }

    float x2k = 2 * drawPoint.x() + 2;
    float y2k = 2 * drawPoint.y() - 2;
    if (pk < 0) {
      drawPoint.set_x(drawPoint.x() + 1);
      pk += x2k + 1;
    } else {
      drawPoint.set(drawPoint.x() + 1, drawPoint.y() - 1);
      pk += x2k + 1 - 2 - y2k;
    }
  }
}

inline float pow2(float number) { return number * number; }

qunt::vector3d calculate_rx_ry(const qunt::point3d& center,
                               const qunt::point3d& p1,
                               const qunt::point3d& p2) {
  float a = pow2(p1.x() - center.x());
  float b = pow2(p1.y() - center.y());
  float c = pow2(p2.x() - center.x());
  float d = pow2(p2.y() - center.y());

  float f = (c * b - d * a) / (c - a);
  float e = (f * a) / (f - b);
  return qunt::vector3d(std::sqrt(e), std::sqrt(f));
}

void drawEllipse(int centerX, int centerY, int ptX1, int ptY1, int ptX2,
                 int ptY2) {
  qunt::vector3d radius =
      calculate_rx_ry({(float)centerX, (float)centerY},
                      {(float)ptX1, (float)ptY1}, {(float)ptX2, (float)ptY2});

  qunt::point3d drawPoint(0, radius.y());

  float p1k =
      pow2(radius.y()) - pow2(radius.x()) * radius.y() + .25 * pow2(radius.x());
  float r2yx = 0;
  float r2xy = 2 * pow2(radius.x()) * radius.y();

  while (r2yx < r2xy) {
    std::vector<qunt::point3d> drawPoints{drawPoint,
                                          {-drawPoint.x(), drawPoint.y()},
                                          {drawPoint.x(), -drawPoint.y()},
                                          {-drawPoint.x(), -drawPoint.y()}};

    for (unsigned i = 0; i < drawPoints.size(); i++) {
      qunt::point3d& p = drawPoints[i];
      drawPixel(centerX + p.x(), centerY + p.y());
    }

    r2yx += 2 * pow2(radius.y());

    if (p1k < 0) {
      drawPoint.set_x(drawPoint.x() + 1);
      p1k += r2yx + pow2(radius.y());
    } else {
      drawPoint.set(drawPoint.x() + 1, drawPoint.y() - 1);
      r2xy -= 2 * pow2(radius.x());
      p1k += r2yx - r2xy + pow2(radius.y());
    }
  }

  float p2k = pow2(radius.y()) * pow2(drawPoint.x() + .5) +
              pow2(radius.x()) * pow2(drawPoint.y() - 1) -
              pow2(radius.x()) * pow2(radius.y());

  while (drawPoint.y() > 0) {
    std::vector<qunt::point3d> drawPoints{drawPoint,
                                          {-drawPoint.x(), drawPoint.y()},
                                          {drawPoint.x(), -drawPoint.y()},
                                          {-drawPoint.x(), -drawPoint.y()}};

    for (unsigned i = 0; i < drawPoints.size(); i++) {
      qunt::point3d& p = drawPoints[i];
      drawPixel(centerX + p.x(), centerY + p.y());
    }

    r2xy -= 2 * pow2(radius.x());

    if (p2k > 0) {
      drawPoint.set_y(drawPoint.y() - 1);
      p2k = p2k - r2xy + pow2(radius.x());
    } else {
      drawPoint.set(drawPoint.x() + 1, drawPoint.y() - 1);
      r2yx += 2 * pow2(radius.y());
      p2k = p2k + r2yx - r2xy + pow2(radius.x());
    }
  }
  // replace above three lines with your code
}

void drawPoly(int ptX1, int ptY1, int ptX2, int ptY2) {
  qunt::point3d p1(ptX1, ptY1);
  qunt::point3d p2(ptX2, ptY2);

  if (p1.x() > p2.x()) {
    std::swap(p1, p2);
  }

  qunt::point3d mid = qunt::get_mid_point(p1, p2);
  qunt::vector3d vec = p2 - p1;
  vec.set_y(std::abs(vec.y()));
  float diff = std::max(vec.x(), vec.y());

  float t0 = p1.y() > p2.y() ? .5 * M_PI : -.5 * M_PI;
  float tn = t0 + M_PI;
  float dT = M_PI / diff;

  float yMult = std::abs((p1.y() - p2.y()) / 2);

  for (float t = t0; t <= tn; t += dT) {
    float currentX = (t - t0) / (tn - t0) * (p2.x() - p1.x()) + p1.x();
    int demoninator = 1;
    float y = 0;
    float yPrev = 100;
    float one = 1;
    float tFixed = t < M_PI ? t : t - 2 * M_PI;
    int n = 1;
    while (std::abs(yPrev - y) > .01 || n < 5) {
      demoninator = n == 1 ? 1 : demoninator * (2 * n - 1) * (2 * n - 2);
      yPrev = y;
      y += (one * std::pow(tFixed, 2 * n - 1) / demoninator);
      one *= -1;
      n++;
    }
    drawPixel(currentX, mid.y() + y * yMult);
  }
}

bool lessThanX(const qunt::point3d& p1, const qunt::point3d& p2) {
  return p1.y() < p2.y();
}

void drawQuinticBezier(int* ptX, int* ptY) {
  std::vector<qunt::point3d> points;
  points.reserve(6);
  int n = 5;
  for (int i = 0; i <= n; i++) {
    points.emplace_back(ptX[i], ptY[i]);
  }

  std::sort(points.begin(), points.end(), lessThanX);
  int distance = 0;
  for (int i = 1; i <= n; i++) {
    distance += (points[i] - points[i - 1]).magnitude();
  }

  std::vector<int> coefficients(n + 1);
  int nFact = factorial(n);
  for (int i = 0; i <= n; i++) {
    coefficients[i] = nFact / factorial(i) / factorial(n - i);
  }

  float dt = 1.0 / distance;
  for (float t = 0; t < 1.0; t += dt) {
    float x = 0, y = 0;
    for (int i = 0; i <= n; i++) {
      float constVal =
          coefficients[i] * std::pow((1 - t), n - i) * std::pow(t, i);
      x += (constVal * points[i].x());
      y += (constVal * points[i].y());
    }

    drawPixel(x, y);
  }
}

// BONUS TASKS
/***************************************************************************/
void drawMidpointParabola(int vertexX, int vertexY, int pointOnParabolaX,
                          int pointOnParabolaY) {
  drawPixel(vertexX, vertexY);
  drawPixel(pointOnParabolaX, pointOnParabolaY);
  // replace above two lines with your code
}

void drawCardinalSpline(int* ptX, int* ptY, int controlPointCount) {
  drawPixel(ptX[0], ptY[0]);
  // replace above line with your code
}

float basisFunction(float i, float t) {
  float u = 0;
  float value = 0;
  if (i <= t && t < i + 1) {
    u = t - i;
    value = 1.0 / 6.0 * std::pow(u, 3);
  } else if (i + 1 <= t && t < i + 2) {
    u = t - (i + 1);
    value = 1.0 / 6.0 * (-3 * std::pow(u, 3) + 3 * pow2(u) + 3 * u + 1);
  } else if (i + 2 <= t && t < i + 3) {
    u = t - (i + 2);
    value = 1.0 / 6.0 * (3 * std::pow(u, 3) - 6 * pow2(u) + 4);
  } else if (i + 3 <= t && t < i + 4) {
    u = t - (i + 3);
    value = 1.0 / 6.0 * (-std::pow(u, 3) + 3 * pow2(u) - 3 * u + 1);
  } else {
    value = 0;
  }

  return value;
}

void drawCubicBSpline(int* ptX, int* ptY, int controlPointCount) {
  int n = controlPointCount;
  std::vector<qunt::point3d> points;
  points.reserve(n);

  for (int i = 0; i < n; i++) {
    points.emplace_back(ptX[i], ptY[i]);
  }

  float dt = 1/200.0;
  for (float t = 4; t <= n; t+=dt) {
    float x = 0, y = 0;

    int i = t - 4;
    //if(i < 0)
    //  continue;

    for (int j = 0; j <= 4; j++) {
      float constVal = basisFunction(i, t);
      x += (constVal * points[i].x());
      y += (constVal * points[i].y());
      i++;
    }

    drawPixel(x, y);
  }
}
