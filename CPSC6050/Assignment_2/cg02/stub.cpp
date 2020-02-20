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

float to_degrees(float radians) {
  return (int)((radians + M_PI) * 180 / M_PI) % 360;
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
  static bool print = true;
  if (print) {
    std::cout << "Vector1: " << line1Vec.x() << " " << line1Vec.y()
              << std::endl;
    std::cout << "Vector2: " << line2Vec.x() << " " << line2Vec.y()
              << std::endl;
    std::cout << "Vector3: " << line3Vec.x() << " " << line3Vec.y()
              << std::endl;

    std::cout << "Angles: " << p1Angle << " " << p2Angle << " " << p3Angle
              << std::endl;
    print = false;
  }
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
  return qunt::vector3d(f, e);
}

void drawEllipse(int centerX, int centerY, int ptX1, int ptY1, int ptX2,
                 int ptY2) {
  drawPixel(ptX1, ptY1);
  drawPixel(ptX2, ptY2);
  drawPixel(centerX, centerY);

  qunt::vector3d radius =
      calculate_rx_ry({centerX, centerY}, {ptX1, ptY1}, {ptX2, ptY2});

  qunt::point3d drawPoint(0, radius.y());
  float p1k = pow2(radius.y()) - pow2(radius.x())* radius.y() + .25 * pow2(radius.x());

  while(true) {

    drawPixel(centerX + drawPoint.x(), centerY + drawPoint.y());
    float r2x
    if(p1k < 0) {
      drawPoint.set_x(drawPoint.x() + 1);
      p1k += 2 * pow2(radius.y())
    }
  }
  // replace above three lines with your code
}

void drawPoly(int ptX1, int ptY1, int ptX2, int ptY2) {
  drawPixel(ptX1, ptY1);
  drawPixel(ptX2, ptY2);
  // replace above two lines with your code
}

void drawQuinticBezier(int* ptX, int* ptY) {
  drawPixel(ptX[0], ptY[0]);
  // replace above line with your code
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

void drawCubicBSpline(int* ptX, int* ptY, int controlPointCount) {
  drawPixel(ptX[0], ptY[0]);
  // replace above line with your code
}
