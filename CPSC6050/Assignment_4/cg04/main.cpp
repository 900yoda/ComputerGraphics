#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "revolution.hpp"
#include "square.hpp"

int WIDTH = 800;
int HEIGHT = 600;
float MOVEMENT = 1.0f;

int windowHandle;

void initialize_window(int* argc, char* argv[]);
void init_opengl();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void mouse(int button, int state, int x, int y);

bool rotate = false;
bool translate = false;
bool scale = false;
bool modelChanged = true;
bool cameraMoved = true;
bool armMoved = true;

glm::mat4 model(1.0f);
glm::mat4 head(1.0f);
glm::mat4 rightArm(1.0f);
glm::mat4 leftArm(1.0f);
glm::mat4 rightLeg(1.0f);
glm::mat4 leftLeg(1.0f);

glm::vec3 rotation(0.0f, 0.0f, 0.0f);
glm::vec3 translation(0.0f, 0.0f, 0.0f);
glm::vec3 scaler(1.0f, 1.0f, 1.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 camera(glm::lookAt(cameraPos, cameraTarget, cameraUp));

GLfloat modelF[16];
GLfloat headF[16];
GLfloat projectionF[16];
GLfloat rightArmF[16];
GLfloat leftArmF[16];
GLfloat rightLegF[16];
GLfloat leftLegF[16];
GLfloat cameraF[16];

glm::mat4 ortho(glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f));
glm::mat4 persp(glm::perspective(glm::radians(45.0f),
                                 (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f));

void transformObject();
void rotateObject();
void translateObject();
void scaleObject();
void translateCamera();
void rotateArm();
void createModel();
void setMatrix(glm::mat4& matrix, GLfloat* matrixF);
void resetMatrix();

bool orthographic = true;
void togglePerspective();

GLUquadric* quadric;

void drawRobot();
void drawBody();
void drawHead();
void drawRightArm();
void drawLeftArm();
void drawRightLeg();
void drawLeftLeg();

void drawCube(const std::vector<square>& cube);
void drawSphere();
void drawCylinder();

glm::vec4 pointsBody[8] = {
    glm::vec4(-1, -1, 1, 1), glm::vec4(1, -1, 1, 1),   glm::vec4(-1, 1, 1, 1),
    glm::vec4(1, 1, 1, 1),   glm::vec4(-1, -1, -1, 1), glm::vec4(1, -1, -1, 1),
    glm::vec4(-1, 1, -1, 1), glm::vec4(1, 1, -1, 1),
};

std::vector<square> body{
    std::vector<glm::vec4>{pointsBody[0], pointsBody[1], pointsBody[2],
                           pointsBody[3]},  // front red
    std::vector<glm::vec4>{pointsBody[4], pointsBody[5], pointsBody[6],
                           pointsBody[7]},  // back green
    std::vector<glm::vec4>{pointsBody[0], pointsBody[4], pointsBody[2],
                           pointsBody[6]},  // left blue
    std::vector<glm::vec4>{pointsBody[1], pointsBody[5], pointsBody[3],
                           pointsBody[7]},  // right yellow
    std::vector<glm::vec4>{pointsBody[2], pointsBody[3], pointsBody[6],
                           pointsBody[7]},  // top purple
    std::vector<glm::vec4>{pointsBody[0], pointsBody[1], pointsBody[4],
                           pointsBody[5]}};  // bottom l blue

enum Mode { ROBOT, DRAW_POINTS, DISPLAY_LINE, SURFACE_R };

std::vector<qunt::point3d> splinePoints;
std::vector<qunt::point3d> drawPoints;
std::vector<glm::vec4> surfacePoints;
std::vector<square> surface;

Mode currentMode = ROBOT;
void swapMode();
void resetAll();
glm::vec4 toVec4(const qunt::point3d& point);

int main(int argc, char* argv[]) {
  head = glm::scale(glm::mat4(1.0f), {0.6f, 0.6f, 0.6f});
  head = glm::translate(glm::mat4(1.0f), {0.0f, 1.55f, 0.0f}) * head;

  rightArm =
      glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {0.0f, 1.0f, 0.0f});
  rightArm = glm::translate(glm::mat4(1.0f), {1.0f, 0.0f, 0.0f}) * rightArm;

  leftArm =
      glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), {0.0f, 1.0f, 0.0f});
  leftArm = glm::translate(glm::mat4(1.0f), {-1.0f, 0.0f, 0.0f}) * leftArm;

  rightLeg =
      glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {1.0f, 0.0f, 0.0f});
  rightLeg = glm::translate(glm::mat4(1.0f), {0.5f, -1.0f, 0.0f}) * rightLeg;

  leftLeg =
      glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {1.0f, 0.0f, 0.0f});
  leftLeg = glm::translate(glm::mat4(1.0f), {-0.5f, -1.0f, 0.0f}) * leftLeg;

  setMatrix(head, headF);
  setMatrix(rightArm, rightArmF);
  setMatrix(leftArm, leftArmF);
  setMatrix(rightLeg, rightLegF);
  setMatrix(leftLeg, leftLegF);

  initialize_window(&argc, argv);

  init_opengl();

  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKey);
  glutMouseFunc(mouse);

  quadric = gluNewQuadric();
  glutMainLoop();

  gluDeleteQuadric(quadric);
  return 0;
}

void initialize_window(int* argc, char* argv[]) {
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  windowHandle = glutCreateWindow("CG4050/6050 A#04: 3D Surfaces And Scenes");
}

void init_opengl() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glMatrixMode(GL_PROJECTION);
  setMatrix(ortho, projectionF);
  glLoadMatrixf(projectionF);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  if (currentMode == ROBOT || currentMode == SURFACE_R) {
    translateCamera();
    transformObject();
  }

  if (currentMode == ROBOT) {
    rotateArm();
    drawRobot();
  }

  if (currentMode == SURFACE_R) {
    resetMatrix();
    glBegin(GL_TRIANGLES);
    {
      float count = 0.0f;
      float size = (float)surface.size();
      bool reverse = false;
      for (const square& sq : surface) {
        float value = count/size;
        glColor3f(value, 1.0f, value);
        for (int i = 0; i < 6; i++) {
          glm::vec4 position = sq[i];
          glVertex3f(position.x, position.y, position.z);
        }

        if(value >= 1) {
          reverse = !reverse;
        }
        count = count + (reverse ? -2.0f : 2.0f);
      }
    }
    glEnd();
  }

  if (currentMode == DRAW_POINTS || currentMode == DISPLAY_LINE) {
    glLoadIdentity();
    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (const qunt::point3d& point : splinePoints) {
      glVertex3f(point.x(), point.y(), 0);
    }
    glEnd();
    glPointSize(1);
  }

  if (currentMode == DISPLAY_LINE) {
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (const qunt::point3d& point : drawPoints) {
      glVertex3f(point.x(), point.y(), 0);
    }
    glEnd();
  }

  glutSwapBuffers();
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  WIDTH = width;
  HEIGHT = height;
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
      rotation.x = 1.0f;
      rotate = true;
      break;
    case 'a':
      rotation.y = 1.0f;
      rotate = true;
      break;
    case 'z':
      rotation.z = 1.0f;
      rotate = true;
      break;
    case 'w':
      scaler.x = 0.9f;
      scale = true;
      break;
    case 'e':
      scaler.x = 1.1f;
      scale = true;
      break;
    case 's':
      scaler.y = 0.9f;
      scale = true;
      break;
    case 'd':
      scaler.y = 1.1f;
      scale = true;
      break;
    case 'x':
      scaler.z = 0.9f;
      scale = true;
      break;
    case 'c':
      scaler.z = 1.1f;
      scale = true;
      break;
    case 'o':
      translation.z = MOVEMENT;
      translate = true;
      break;
    case 'l':
      translation.z = -MOVEMENT;
      translate = true;
      break;
    case 'r':
      resetAll();
      break;
    case 'k':
      translation.x = -MOVEMENT;
      cameraMoved = true;
      break;
    case 'h':
      translation.x = MOVEMENT;
      cameraMoved = true;
      break;
    case 'u':
      translation.y = MOVEMENT;
      cameraMoved = true;
      break;
    case 'j':
      translation.y = -MOVEMENT;
      cameraMoved = true;
      break;
    case 'y':
      translation.z = MOVEMENT;
      cameraMoved = true;
      break;
    case 'i':
      translation.z = -MOVEMENT;
      cameraMoved = true;
      break;
    case 'f':
      togglePerspective();
      break;
    case '1':
      rotation.z = 1.0f;
      armMoved = true;
      break;
    case '2':
      rotation.z = -1.0f;
      armMoved = true;
      break;
    case '3':
      rotation.y = 1.0f;
      armMoved = true;
      break;
    case '4':
      rotation.y = -1.0f;
      armMoved = true;
      break;
    case 'v':
      swapMode();
      break;
    case 27:  // ESC
      if (currentMode == DRAW_POINTS) {
      }
      break;
    default:
      std::cout << (int)key << std::endl;
      break;
  }
}

void specialKey(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      translation.x = -MOVEMENT;
      translate = true;
      break;
    case GLUT_KEY_RIGHT:
      translation.x = MOVEMENT;
      translate = true;
      break;
    case GLUT_KEY_UP:
      translation.y = MOVEMENT;
      translate = true;
      break;
    case GLUT_KEY_DOWN:
      translation.y = -MOVEMENT;
      translate = true;
      break;
  }
}

void mouse(int button, int state, int x, int y) {
  if (currentMode == DRAW_POINTS && button == GLUT_LEFT_BUTTON &&
      state == GLUT_DOWN) {
    float xPos = (float)x / WIDTH;
    float yPos = 1 - (float)y / HEIGHT;
    splinePoints.emplace_back(xPos, yPos);
    std::cout << splinePoints.back() << std::endl;
    if (splinePoints.size() == 6) {
      currentMode = DISPLAY_LINE;
      drawPoints = getQuinticBezier(splinePoints, WIDTH, HEIGHT);
    }
  }

  if (currentMode == DISPLAY_LINE && button == GLUT_LEFT_BUTTON &&
      state == GLUT_UP) {
    int i, j;
    std::cout << "Enter i value (number of steps along the y): ";
    std::cin >> i;
    std::cout << "Enter j value (number of radial axes): ";
    std::cin >> j;
    orthographic = !orthographic;
    togglePerspective();
    currentMode = SURFACE_R;

    int dt = (int)drawPoints.size() / i;
    for (int k = 0, p = 0; k < i; k++, p += dt) {
      qunt::point3d& point = drawPoints[p];
      surfacePoints.emplace_back(toVec4(point));
    }

    surfacePoints.emplace_back(toVec4(drawPoints.back()));

    float rotation = glm::radians(360.0f / j);
    int size = surfacePoints.size();
    std::vector<glm::vec4> prev = surfacePoints;
    for (int k = 1; k <= j; k++) {
      std::vector<glm::vec4> curr;
      curr.reserve(size);
      for (const glm::vec4& point : surfacePoints) {
        int mult = k == j ? 0 : k;
        float x = point.x * std::cos(rotation * mult);
        float y = point.y;
        float z = point.x * std::sin(rotation * mult);
        curr.emplace_back(x, y, z, 0.0f);
      }

      for (int p = 1; p <= size; p++) {
        surface.emplace_back(curr[p % size], curr[p - 1], prev[p % size],
                             prev[p - 1]);
      }

      prev = curr;
    }
  }
}

void transformObject() {
  rotateObject();
  translateObject();
  scaleObject();
  createModel();
}

void rotateObject() {
  if (!rotate) return;

  model = glm::rotate(glm::mat4(1.0f), glm::radians(2.0f), rotation) * model;
  rotate = false;
  rotation = glm::vec3(0.0, 0.0, 0.0);
  modelChanged = true;
}

void translateObject() {
  if (!translate) return;

  model = glm::translate(glm::mat4(1.0f), translation) * model;
  translate = false;
  translation = glm::vec3(0.0, 0.0, 0.0);
  modelChanged = true;
}

void scaleObject() {
  if (!scale) return;

  model = glm::scale(glm::mat4(1.0f), scaler) * model;
  scale = false;
  scaler = glm::vec3(1.0f, 1.0f, 1.0f);
  modelChanged = true;
}

void translateCamera() {
  if (!cameraMoved) return;

  camera = glm::translate(camera, -1.0f * translation);
  setMatrix(camera, cameraF);
  translation = glm::vec3(0.0, 0.0, 0.0);
  cameraMoved = false;
}

void rotateArm() {
  if (!armMoved) return;

  if (rotation != glm::vec3(0.0f, 0.0f, 0.0f)) {
    rightArm =
        glm::rotate(glm::mat4(1.0f), glm::radians(2.0f), rotation) * rightArm;
  }

  setMatrix(rightArm, rightArmF);
  rotation = glm::vec3(0.0, 0.0, 0.0);
  armMoved = false;
}

void setMatrix(glm::mat4& matrix, GLfloat* matrixF) {
  int p = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrixF[p++] = matrix[i][j];
    }
  }
}

void resetMatrix() {
  glLoadIdentity();
  glMultMatrixf(cameraF);
  glMultMatrixf(modelF);
}

void createModel() {
  if (!modelChanged) return;

  setMatrix(model, modelF);
  modelChanged = false;
}

void togglePerspective() {
  glm::mat4& matrix = orthographic ? persp : ortho;
  glMatrixMode(GL_PROJECTION);
  setMatrix(matrix, projectionF);
  glLoadMatrixf(projectionF);

  orthographic = !orthographic;
}

void drawRobot() {
  drawBody();
  drawHead();
  drawRightArm();
  drawLeftArm();
  drawRightLeg();
  drawLeftLeg();
}

void drawBody() {
  resetMatrix();
  drawCube(body);
}

void drawHead() {
  resetMatrix();
  glMultMatrixf(headF);
  drawSphere();
}

void drawRightArm() {
  resetMatrix();
  glMultMatrixf(rightArmF);
  drawCylinder();
}

void drawLeftArm() {
  resetMatrix();
  glMultMatrixf(leftArmF);
  drawCylinder();
}

void drawRightLeg() {
  resetMatrix();
  glMultMatrixf(rightLegF);
  drawCylinder();
}

void drawLeftLeg() {
  resetMatrix();
  glMultMatrixf(leftLegF);
  drawCylinder();
}

void drawCube(const std::vector<square>& cube) {
  glBegin(GL_TRIANGLES);
  {
    std::vector<glm::vec3> colors{
        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)};

    int j = 0;
    for (const square& sq : cube) {
      glm::vec3& color = colors[j++];
      glColor3f(color.x, color.y, color.z);

      for (int i = 0; i < 6; i++) {
        glm::vec4 position = sq[i];
        glVertex3f(position.x, position.y, position.z);
      }
    }
  }
  glEnd();
}

void drawSphere() {
  glColor3f(1.0f, 0.0f, 0.0f);
  gluSphere(quadric, 1.0f, 20, 20);
}

void drawCylinder() {
  glColor3f(1.0f, 0.0f, 0.0f);
  gluCylinder(quadric, 0.25f, 0.25f, 2.0f, 20, 20);
}

void swapMode() {
  if (currentMode == ROBOT) {
    currentMode = DRAW_POINTS;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1.0, 1.0);
    std::cout << "Please use the mouse plot 6 points" << std::endl;
    splinePoints.clear();
    drawPoints.clear();
    surfacePoints.clear();
    surface.clear();
  } else {
    orthographic = !orthographic;
    togglePerspective();
    currentMode = ROBOT;
  }

  resetAll();
}

void resetAll() {
  model = glm::mat4(1.0f);
  camera = glm::lookAt(cameraPos, cameraTarget, cameraUp);
  rightArm =
      glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {0.0f, 1.0f, 0.0f});
  rightArm = glm::translate(glm::mat4(1.0f), {1.0f, 0.0f, 0.0f}) * rightArm;
  modelChanged = true;
  cameraMoved = true;
  armMoved = true;
}

glm::vec4 toVec4(const qunt::point3d& point) {
  return glm::vec4(point.x(), point.y(), point.z(), 0);
}