#include <GL/glut.h>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CUBE_SIZE = 2.0f;
const float SPHERE_RADIUS = 1.0f;
const float CYLINDER_RADIUS = 0.5f;
const float CYLINDER_HEIGHT = 1.5f;
static float rotationAngle = 0.0f;

void drawWireCylinder(float radius, float height, int slices, int stacks)
{
  (void)stacks;
  float halfH = height * 0.5f;
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < slices; ++i)
  {
    float angle = 2.0f * M_PI * i / slices;
    glVertex3f(radius * cos(angle), -halfH, radius * sin(angle));
  }
  glEnd();

  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < slices; ++i)
  {
    float angle = 2.0f * M_PI * i / slices;
    glVertex3f(radius * cos(angle), halfH, radius * sin(angle));
  }
  glEnd();

  glBegin(GL_LINES);
  for (int i = 0; i < slices; ++i)
  {
    float angle = 2.0f * M_PI * i / slices;
    float x = radius * cos(angle);
    float z = radius * sin(angle);
    glVertex3f(x, -halfH, z);
    glVertex3f(x, halfH, z);
  }
  glEnd();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(5.0, 4.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

  glColor3f(0.0f, 0.0f, 1.0f);
  glutWireCube(CUBE_SIZE);
  glColor3f(0.0f, 1.0f, 0.0f);
  glutWireSphere(SPHERE_RADIUS, 20, 20);

  glPushMatrix();
  {
    glTranslatef(3.5f, 0.0f, 0.0f);
    glScalef(2.5f, 2.5f, 2.5f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutWireSphere(SPHERE_RADIUS, 20, 20);
  }
  glPopMatrix();

  glPushMatrix();
  {
    glTranslatef(-3.5f, 0.0f, 0.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    {
      glTranslatef(1.5f, 0.0f, 0.0f);
      drawWireCylinder(CYLINDER_RADIUS * 1.5f, CYLINDER_HEIGHT, 20, 10);
    }
    glPopMatrix();

    glColor3f(0.0f, 1.0f, 1.0f);
    glPushMatrix();
    {
      glTranslatef(-1.5f, 0.0f, 0.0f);
      glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
      drawWireCylinder(CYLINDER_RADIUS, CYLINDER_HEIGHT, 20, 10);
    }
    glPopMatrix();
  }
  glPopMatrix();

  glutSwapBuffers();
}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double)width / (double)height, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27:
      exit(0);
    case 'r':
    case 'R':
      rotationAngle += 15.0f; // увеличенный шаг
      if (rotationAngle > 360.0f)
        rotationAngle -= 360.0f;
      glutPostRedisplay();
      break;
  }
}

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("OpenGL: Cube, Sphere, Cylinders");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat lightPosition[] = {5.0f, 5.0f, 5.0f, 1.0f};
  GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}
