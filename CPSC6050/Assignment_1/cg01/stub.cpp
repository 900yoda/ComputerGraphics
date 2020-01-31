//
//		          Programming Assignment #1
//
//					Daljit Singh Dhillon
//
//
/***************************************************************************/
/* Include needed files */
#include <cmath>
/***************************************************************************/

struct vec3d {
  float x, y, z;
  vec3d() : x(0.0), y(0.0), z(0.0) {}
  vec3d(float x, float y) : x(x), y(y), z(0.0) {}
  vec3d(float x, float y, float z) : x(x), y(y), z(z) {}
  vec3d operator/(float num) { return vec3d(x / num, y / num, z / num); }
  vec3d operator*(float num) { return vec3d(x * num, y * num, z * num); }
  vec3d operator+(const vec3d &vec) {
    return vec3d(x + vec.x, y + vec.y, z + vec.z);
  }
  float length() { return (float)std::sqrt(x * x + y * y + z * z); }
};

struct point {
  int x, y, z;
  point() : x(0), y(0), z(0) {}
  point(int x, int y) : x(x), y(y), z(0) {}
  point(int x, int y, int z) : x(x), y(y), z(z) {}
  point operator+(const vec3d &vec) {
    return point(x + vec.x, y + vec.y, z + vec.z);
  }
  point operator-(const vec3d &vec) { return ((*this) + (vec3d(vec) * -1)); }
  vec3d operator-(const point &p) { return vec3d(x - p.x, y - p.y, z - p.z); }
};

enum LineType { NORMAL, DASHED, DOTTED };

static LineType style = NORMAL;
static int prevLength = 100;
static int xOrigin = 0, yOrigin = 0;
static int thickness = 1;
static vec3d slope;
static point lastDrawn;
static bool drawnYet = false;

// Forward declarations
void drawPixel(int x, int y);
void drawPixel(point start, point loc);
void drawAxis(vec3d vec);
void setSlope(point p1, point p2);
void drawLineDDA(float xStart, float yStart, float xEnd, float yEnd);
void drawLineBresenham(point p1, point p2);
void drawLineBresenham(int xStart, int yStart, int xEnd, int yEnd);
void drawLineBresenhamX(int dx, int dy, point loc);
void drawLineBresenhamY(int dx, int dy, point loc);
void drawThickLineBresenham(int xStart, int yStart, int xEnd, int yEnd,
                            int lineWidth);
void drawLineSlopeIntercept(float slope, float yIntercept);
void draw2dAxesAndSquare(float orgX, float orgY, float xVecE1, float xVecE2,
                         float yVecE1, float yVecE2, float sqrBaseX,
                         float sqrBaseY, float sqrWidth);
void draw3dAxesAndCube(float orgX, float orgY, float xVecE1, float xVecE2,
                       float yVecE1, float yVecE2, float zVecE1, float zVecE2,
                       float cubeBaseX, float cubeBaseY, float cubeBaseZ,
                       float cubeWidth);
void drawStyleDottedLine(float xStart, float yStart, float xEnd, float yEnd,
                         int lineWidth);
void drawStyleDashedLine(float xStart, float yStart, float xEnd, float yEnd,
                         int lineWidth);
void drawStyleDotAndDashLine(float xStart, float yStart, float xEnd, float yEnd,
                             int lineWidth);

inline void swap(point &p1, point &p2) {
  point temp = p1;
  p1 = p2;
  p2 = temp;
}

void drawPixel(point start, point loc) {
  int length = (int)(start - loc).length();
  if (style == NORMAL || (style == DASHED && length % 20 < 10)) {
    if (thickness == 1) {
      drawPixel(loc.x + xOrigin, loc.y + yOrigin);
    } else {
      int tempThickness = thickness;
      thickness = 1;
      point p1 = loc + slope * tempThickness / 2.0;
      point p2 = loc - slope * tempThickness / 2.0;
      drawLineBresenham(p1, p2);
      thickness = tempThickness;
    }

  } else if (style == DOTTED) {
    if (thickness == 1) {
      length %= 10;
      if (length < prevLength) {
        drawPixel(loc.x + xOrigin, loc.y + yOrigin);
      }
      prevLength = length;
    } else {
      if (!drawnYet || (loc - lastDrawn).length() > thickness + 10) {
        
        lastDrawn = loc;
        int radius = thickness / 2;
        double rSquared = std::pow(radius, 2);

        for (int y = 0; y < radius; y++) {
          int x = int(std::sqrt(rSquared - std::pow(y, 2)));
          int x1 = loc.x + x;
          int x2 = loc.x - x;
          int y1 = loc.y + y;
          int y2 = loc.y - y;
          style = NORMAL;
          int tempThickness = thickness;
          thickness = 1;
          drawLineBresenham(x2, y1, x1, y1);
          drawLineBresenham(x2, y2, x1, y2);
          style = DOTTED;
          thickness = tempThickness;
        }

        drawnYet = true;
      }
    }
  }
}

void drawAxis(vec3d vec) {
  if (vec.x == 0.0) {
    drawLineBresenhamY(0, 600, point(0, -yOrigin));
  } else {
    drawLineSlopeIntercept(vec.y / vec.x, 0);
  }
}

void drawLineDDA(float xStart, float yStart, float xEnd, float yEnd) {
  drawPixel((int)xStart, (int)yStart);
  drawPixel((int)xEnd, (int)yEnd);

  // replace above two lines with your own inplementation
}

void drawLineBresenham(point p1, point p2) {
  drawLineBresenham(p1.x, p1.y, p2.x, p2.y);
}

void drawLineBresenham(int xStart, int yStart, int xEnd, int yEnd) {
  drawnYet = false;
  prevLength = 100;
  point p0(xStart, yStart);
  point p1(xEnd, yEnd);

  int dx = p1.x - p0.x;
  int dy = p1.y - p0.y;

  if (dx == 0 || std::abs((float)dy / (float)dx) > 1.0) {
    if (p1.y < p0.y) swap(p0, p1);

    dx = p1.x - p0.x;
    dy = p1.y - p0.y;
    drawLineBresenhamY(dx, dy, p0);
  } else {
    if (p1.x < p0.x) swap(p0, p1);

    dx = p1.x - p0.x;
    dy = p1.y - p0.y;
    drawLineBresenhamX(dx, dy, p0);
  }
}

void drawLineBresenhamX(int dx, int dy, point loc) {
  point start(loc);
  int dy2 = std::abs(dy * 2);
  int dy2mdx2 = dy2 - (dx * 2);

  int pk = dy2 - dx;
  int yShift = dy > 0 ? 1 : -1;
  drawPixel(loc.x + xOrigin, loc.y + yOrigin);
  for (int i = 0; i < dx; i++) {
    loc.x++;
    if (pk < 0) {
      drawPixel(start, loc);
      pk += dy2;
    } else {
      loc.y += yShift;
      drawPixel(start, loc);
      pk += dy2mdx2;
    }
  }
}

void drawLineBresenhamY(int dx, int dy, point loc) {
  point start(loc);
  int dx2 = std::abs(dx * 2);
  int dx2mdy2 = dx2 - (dy * 2);

  int pk = dx2 - dy;
  int xShift = dx > 0 ? 1 : -1;
  drawPixel(loc.x + xOrigin, loc.y + yOrigin);
  for (int i = 0; i < dy; i++) {
    loc.y++;
    if (pk < 0) {
      drawPixel(start, loc);
      pk += dx2;
    } else {
      loc.x += xShift;
      drawPixel(start, loc);
      pk += dx2mdy2;
    }
  }
}

void drawThickLineBresenham(int xStart, int yStart, int xEnd, int yEnd,
                            int lineWidth) {
  point start(xStart, yStart);
  point end(xEnd, yEnd);

  slope = vec3d(end - start);
  slope = vec3d(-slope.y, slope.x);
  slope = slope / slope.length();
  thickness = lineWidth;

  drawLineBresenham(start, end);
  thickness = 1;
}

void drawStyleDottedLine(float xStart, float yStart, float xEnd, float yEnd,
                         int lineWidth) {
  style = DOTTED;
  thickness = lineWidth;
  drawLineBresenham(xStart, yStart, xEnd, yEnd);
  thickness = 1;
  style = NORMAL;
}

void drawStyleDashedLine(float xStart, float yStart, float xEnd, float yEnd,
                         int lineWidth) {
  style = DASHED;
  drawThickLineBresenham(xStart, yStart, xEnd, yEnd, lineWidth);
  style = NORMAL;
}

void drawStyleDotAndDashLine(float xStart, float yStart, float xEnd, float yEnd,
                             int lineWidth) {
  drawPixel((int)xStart, (int)yStart);
  drawPixel((int)xEnd, (int)yEnd);

  // replace above two lines with your own inplementation
}

void drawLineSlopeIntercept(float slope, float yIntercept) {
  point p0(0 - xOrigin, (int)((0 - xOrigin) * slope) + yIntercept);
  point p1(800 - xOrigin, (int)((800 - xOrigin) * slope) + yIntercept);
  drawLineBresenham(p0.x, p0.y, p1.x, p1.y);
}
void draw2dAxesAndSquare(float orgX, float orgY, float xVecE1, float xVecE2,
                         float yVecE1, float yVecE2, float cubeBaseX,
                         float cubeBaseY, float cubeWidth) {
  drawPixel((int)orgX, (int)orgY);
  drawPixel((int)orgX, (int)orgY);

  // replace above two lines with your own inplementation
}
void draw3dAxesAndCube(float orgX, float orgY, float xVecE1, float xVecE2,
                       float yVecE1, float yVecE2, float zVecE1, float zVecE2,
                       float cubeBaseX, float cubeBaseY, float cubeBaseZ,
                       float cubeWidth) {
  xOrigin = orgX;
  yOrigin = orgY;

  vec3d xAxis(xVecE1, xVecE2);
  vec3d yAxis(yVecE1, yVecE2);
  vec3d zAxis(zVecE1, zVecE2);

  drawAxis(xAxis);
  drawAxis(yAxis);
  drawAxis(zAxis);

  vec3d xHat = xAxis / xAxis.length();
  vec3d yHat = yAxis / yAxis.length();
  vec3d zHat = zAxis / zAxis.length();

  point p1 =
      point(0, 0) + (xHat * cubeBaseX + yHat * cubeBaseY + zHat * cubeBaseZ);
  point p2 = p1 + xHat * cubeWidth;
  point p3 = p1 + yHat * cubeWidth;
  point p4 = p3 + xHat * cubeWidth;

  point p5 = p1 + zHat * cubeWidth;
  point p6 = p2 + zHat * cubeWidth;
  point p7 = p3 + zHat * cubeWidth;
  point p8 = p4 + zHat * cubeWidth;

  drawLineBresenham(p1, p2);
  drawLineBresenham(p1, p3);
  drawLineBresenham(p1, p5);
  drawLineBresenham(p2, p4);
  drawLineBresenham(p2, p6);
  drawLineBresenham(p3, p4);
  drawLineBresenham(p3, p7);
  drawLineBresenham(p4, p8);
  drawLineBresenham(p5, p6);
  drawLineBresenham(p5, p7);
  drawLineBresenham(p6, p8);
  drawLineBresenham(p7, p8);

  xOrigin = 0;
  yOrigin = 0;
}
