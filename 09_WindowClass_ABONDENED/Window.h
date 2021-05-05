#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "GL/freeglut.h"
#endif
#include "GL/gl.h"
#include <math.h>
#include <vector>

int FPS = 60;

class Object{
    private:
    public:
    Object(){};
    void render(){};
    ~Object(){};
}



class Window
{
private:
    int WINDOW_WIDTH, WINDOW_HEIGHT;
public:
    Window(int argc, char **argv, int width, int height);
    ~Window();
};

void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINE_LOOP);
    glColor3f(1, 0, 0);
    glVertex2d(0, 0);
    glVertex2d(0.5, 0.5);
    glVertex2d(0.5, 0.75);
    glEnd();

    glutSwapBuffers();
    glFlush();
}

Window::Window(int argc, char **argv, int width, int height)
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Window");

    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);

    glutMainLoop();
}

Window::~Window()
{
}
