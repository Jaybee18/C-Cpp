#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "gl/freeglut.h"
#endif
#include "GL/gl.h"
#include <math.h>
#include <iostream>
//#include "birb.h"

#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "gl/freeglut.h"
#endif
#include "GL/gl.h"
#include <math.h>

void drawCircle(int x, double y, int radius)
{
    double center[2] = {x / 900.0, y / 900.0};
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 0);
    for (int i = 0; i < 360; i++)
    {
        double yDelta = radius * sin(i);
        double xDelta = radius * cos(i);
        double point[2] = {center[0] + xDelta, center[1] + yDelta};
        glVertex3f(point[0] / 900.0, point[1] / 900.0, 0);
    }
    glEnd();
    glutSwapBuffers();
}
class Birb
{
private:
    int x, y;
    double ydelta, g;
public:
    Birb();
    Birb(int startingX, int startingY);
    void update();
    void draw();
    void test();
};

Birb::Birb()
{
    x = 0;
    y = 0;
    ydelta = 0.01;
    g = 0.001;
}

Birb::Birb(int startingX, int startingY)
{
    x = startingX;
    y = startingY;
    ydelta = 0.01;
    g = 0.001;
}

void Birb::update()
{
    y += 0.01;
    glPushMatrix();
    glTranslatef(0, y, 0);
    drawCircle(40, 40, 200);
    glPopMatrix();
}

void Birb::draw()
{
    double center[2] = {0, 0};
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 0);
    for (int i = 0; i < 360; i++)
    {
        double yDelta = 200.0 * sin(i);
        double xDelta = 200.0 * cos(i);
        double point[2] = {center[0] + xDelta, center[1] + yDelta};
        glVertex3f(point[0] / 900.0, point[1] / 900.0, 0);
    }
    glEnd();
    glutSwapBuffers();
}

/* compiling
 * linux   : g++ 05_opengl/main.cpp -lglut -lGL -o 05_opengl/main; ./05_opengl/main
 * windows : g++ 05_opengl/main.cpp -o 05_opengl/main.exe -I"C:\MinGW\include" -L"C:\MinGW\lib" -lfreeglut -lopengl32 && 05_opengl\main.exe
 */
static const int FPS = 60;
double yposition = 0.0;
bool move = true;
Birb b = Birb();

void drawTriangle()
{
    glBegin(GL_POLYGON);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0.5, 0.1, 0);
    glVertex3f(0.2, 0.5, 0);
    glEnd();
}


void timer(int v)
{
    //if (move)
    //    b.test();
    //yposition += 0.01;
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        move = true;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        move = false;
    }
}

void display2()
{
    // Set every pixel in the frame buffer to the current clear color. (resets the frame)
    glClear(GL_COLOR_BUFFER_BIT);
    //drawTriangle();

    yposition += 0.01;
    glPushMatrix();
    glTranslatef(0, yposition, 0);
    drawCircle(40, 40, 200);
    glPopMatrix();

    glFlush();
}

void display()
{
    // Set every pixel in the frame buffer to the current clear color. (resets the frame)
    glClear(GL_COLOR_BUFFER_BIT);
    
    yposition += 0.01;
    glPushMatrix();
    glTranslatef(0, yposition, 0);
    drawCircle(40, 40, 200);
    glPopMatrix();  

    glFlush();
}

int main(int argc, char **argv)
{
    /* initialisation */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Fappy Birb");

    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}