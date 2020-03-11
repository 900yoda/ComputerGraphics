#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int WIDTH = 800;
int HEIGHT = 600;
float HALFWIDTH = WIDTH / 2.0;
float HALFHEIGHT = HEIGHT / 2.0;
float MOVEMENT = 1.0f;

int windowHandle;

void initialize_window(int* argc, char* argv[]);
void init_opengl();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);

bool rotate = false;
bool translate = false;
bool scale = false;
bool modelChanged = true;
bool cameraMoved = true;

glm::mat4 model(1.0f);
glm::vec3 rotation(0.0f, 0.0f, 0.0f);
glm::vec3 translation(0.0f, 0.0f, 0.0f);
glm::vec3 scaler(1.0f, 1.0f, 1.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 10.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 camera(glm::lookAt(cameraPos, cameraTarget, cameraUp));

GLfloat modelF[16];
GLfloat cameraF[16];
GLfloat projectionF[16];

glm::mat4 ortho(glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f));
glm::mat4 persp(glm::perspective(glm::radians(45.0f),
                                 (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f));

void transformObject();
void rotateObject();
void translateObject();
void scaleObject();
void translateCamera();
void createModel();
void setMatrix(glm::mat4& matrix, GLfloat* matrixF);

bool orthographic = true;
void togglePerspective();

enum SHAPE { SPHERE, CUBE, CYLINDER };
SHAPE shape = SHAPE::CUBE;
GLUquadric* quadric;
void drawShape();
void drawSphere();
void drawCube();
void drawCylinder();

class square {
 private:
  std::vector<glm::vec4> points;

 public:
  square(const std::vector<glm::vec4>& points) : points(points) {}

  glm::vec4& operator[](int i) {
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
};

float multiplier = 1.0f;

glm::vec4 points[8] = {
    glm::vec4(-1, -1, 1, 1), glm::vec4(1, -1, 1, 1),   glm::vec4(-1, 1, 1, 1),
    glm::vec4(1, 1, 1, 1),   glm::vec4(-1, -1, -1, 1), glm::vec4(1, -1, -1, 1),
    glm::vec4(-1, 1, -1, 1), glm::vec4(1, 1, -1, 1),
};

std::vector<square> faces{
    std::vector<glm::vec4>{points[0], points[1], points[2],
                           points[3]},  // front red
    std::vector<glm::vec4>{points[4], points[5], points[6],
                           points[7]},  // back green
    std::vector<glm::vec4>{points[0], points[4], points[2],
                           points[6]},  // left blue
    std::vector<glm::vec4>{points[1], points[5], points[3],
                           points[7]},  // right yellow
    std::vector<glm::vec4>{points[2], points[3], points[6],
                           points[7]},  // top purple
    std::vector<glm::vec4>{points[0], points[1], points[4],
                           points[5]}};  // bottom l blue

int main(int argc, char* argv[]) {
  initialize_window(&argc, argv);

  init_opengl();

  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKey);

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
  windowHandle =
      glutCreateWindow("CG4050/6050 A#03: 3D Object Transformations");
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
  glLoadIdentity();
  translateCamera();
  transformObject();
  glMultMatrixf(cameraF);
  glMultMatrixf(modelF);
  if (shape == CYLINDER) glTranslatef(0.0f, 0.0f, -1.0f);
  drawShape();
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
      model = glm::mat4(1.0f);
      camera = glm::lookAt(cameraPos, cameraTarget, cameraUp);
      modelChanged = true;
      cameraMoved = true;
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
    case 'v':
      shape = CUBE;
      break;
    case 't':
      shape = SPHERE;
      break;
    case 'g':
      shape = CYLINDER;
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

void transformObject() {
  rotateObject();
  translateObject();
  scaleObject();
  createModel();
  translateCamera();
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

void setMatrix(glm::mat4& matrix, GLfloat* matrixF) {
  int p = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrixF[p++] = matrix[i][j];
    }
  }
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

void drawShape() {
  switch (shape) {
    case CUBE:
      drawCube();
      break;
    case SPHERE:
      drawSphere();
      break;
    case CYLINDER:
      drawCylinder();
      break;
    default:
      break;
  }
}

void drawSphere() {
  glColor3f(1.0f, 0.0f, 0.0f);
  gluSphere(quadric, 1.0f, 20, 20);
}

void drawCube() {
  glBegin(GL_TRIANGLES);
  {
    std::vector<glm::vec3> colors{
        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)};

    int j = 0;
    for (square& sq : faces) {
      glm::vec3& color = colors[j++];
      glColor3f(color.x, color.y, color.z);
      for (unsigned int i = 0; i < 6; i++) {
        glm::vec4 position = multiplier * sq[i];
        glVertex3f(position.x, position.y, position.z);
      }
    }
  }
  glEnd();
}

void drawCylinder() {
  glColor3f(1.0f, 0.0f, 0.0f);
  gluCylinder(quadric, 1.0f, 1.0f, 2.0f, 20, 20);
}