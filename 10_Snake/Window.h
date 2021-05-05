#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "GL/freeglut.h"
#endif
#include "GL/gl.h"
#include <math.h>
#include <vector>
#include <iostream>

/* constants */
static int FPS = 60;
static void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

/* objects */
struct Rectangle{
    int x, y;
    double width, height;
};

/* main class */
class Window
{
private:
    int WINDOW_WIDTH, WINDOW_HEIGHT;
    Rectangle *test;
public:
    Window(){};
    Window(int argc, char **argv, int width, int height, void display());
    void drawRect(Rectangle rect);
    void update();
    ~Window();
};

Window::Window(int argc, char **argv, int width, int height, void display())
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    test = new Rectangle();
    test->x = 0;
    test->y = 0;
    test->width = 0.5;
    test->height = 0.5;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Window");

    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);

    glutMainLoop();
}

void Window::drawRect(Rectangle rect){
    glBegin(GL_LINE_LOOP);
    glVertex2d(rect.x, rect.y);
    glVertex2d(rect.x + rect.width, rect.y);
    glVertex2d(rect.x + rect.width, rect.y + rect.height);
    glVertex2d(rect.x, rect.y + rect.height);
    glEnd();
};

void Window::update(){
    glClear(GL_COLOR_BUFFER_BIT);

    Rectangle *rect = test;
    std::cout << rect->width << std::endl;
    glBegin(GL_LINE_LOOP);
    glVertex2d(rect.x, rect.y);
    glVertex2d(rect.x + rect.width, rect.y);
    glVertex2d(rect.x + rect.width, rect.y + rect.height);
    glVertex2d(rect.x, rect.y + rect.height);
    glEnd();
    //drawRect(test);

    glutSwapBuffers();
    glFlush();
}

Window::~Window()
{
}
