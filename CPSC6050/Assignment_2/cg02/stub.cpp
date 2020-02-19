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
  drawPixel(ptX1, ptY1);
  drawPixel(ptX2, ptY2);
  drawPixel(ptX3, ptY3);

  // replace above three lines with your code
  qunt::point3d p1(ptX1, ptY1);
  qunt::point3d p2(ptX2, ptY2);
  qunt::point3d p3(ptX3, ptY3);

  // get the two points for the line for the first bisector
  qunt::point3d m1 = qunt::get_mid_point(p1, p3);
  qunt::vector3d v1 = p3 - p1;
  v1 = qunt::vector3d(-v1.y(), v1.x());
  qunt::point3d m2 = m1 + v1;

  // get the two points for the line for the second bisector
  qunt::point3d m3 = qunt::get_mid_point(p2, p3);
  qunt::vector3d v2 = p3 - p2;
  v2 = qunt::vector3d(-v2.y(), v2.x());
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

  float radius = (center - p1).magnitude();
  qunt::point3d drawPoint(0, radius);
  float pk = 5.0 / 4 - radius;

  while (drawPoint.x() < drawPoint.y()) {
	  drawPixel(center.x() + drawPoint.x(), center.y() + drawPoint.y());
	  float x2k = 2*drawPoint.x() + 2;
	  float y2k = 2*drawPoint.y() - 2;
	  if(pk < 0) {
		  drawPoint = qunt::point3d(drawPoint.x() + 1, drawPoint.y());
		  pk += x2k + 1;
	  } else {
		  drawPoint = qunt::point3d(drawPoint.x() + 1, drawPoint.y() - 1);
		  pk += x2k + 1 - 2 - y2k;
	  }
  }
}

void drawEllipse(int centerX, int centerY, int ptX1, int ptY1, int ptX2,
                 int ptY2) {
  drawPixel(ptX1, ptY1);
  drawPixel(ptX2, ptY2);
  drawPixel(centerX, centerY);
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
