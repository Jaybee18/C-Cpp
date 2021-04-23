#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "gl/freeglut.h"
#endif
#include "GL/gl.h"
#include <math.h>
#include <iostream>
#include "plist.h"

#define drawOneLine(x1, y1, x2, y2) \
    glBegin(GL_LINES);              \
    glVertex3f((x1), (y1), (-5.0)); \
    glVertex3f((x2), (y2), (-5.0)); \
    glEnd();

/* birb */
struct Birb
{
    double x = -500.0, y = 0.0;
    double velocity = 0.03;
    bool isDead = false;
    double width = 100.0;
};

/* pipes */
struct Pipe
{
    double x;
    double height;
    double width = 0.2;
};

/* general variables/constants */
static int WINDOW_WIDTH = 900, WINDOW_HEIGHT = 900;
static double PIPE_GAP_HEIGHT = 800.0/WINDOW_HEIGHT;
static double PIPE_MOVEMENT_SPEED = 0.008;
static double GRAVITY = 0.0015;
static int FPS = 60;
int score = 0;
double distToNextPipe = 0.0;
double yPosittion = 0.0;
double yOfNextHole = 0.0;
Pipe currentLastPipe;
list<Pipe> pipes;
Birb birb = Birb();


/* shapes and figures */
void drawCircle(int x, double y, int radius)
{
    double center[2] = {x * 1.0, y * 1.0};
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 1);
    for (int i = 0; i < 360; i++)
    {
        double yDelta = radius * sin(i);
        double xDelta = radius * cos(i);
        double point[2] = {center[0] + xDelta, center[1] + yDelta};
        glVertex3f(point[0] / WINDOW_WIDTH, point[1] / WINDOW_HEIGHT, 0);
    }
    glEnd();
}

void drawPipe(Pipe p){
    /* lower half */
    glBegin(GL_POLYGON);
    glColor3f(0, 1, 0);
    glVertex3f(p.x, -1.0, 0);
    glVertex3f(p.x + p.width, -1.0, 0);
    glVertex3f(p.x + p.width, -1.0 + p.height, 0);
    glVertex3f(p.x, -1.0 + p.height, 0);
    glEnd();
    /* upper half */
    glBegin(GL_POLYGON);
    glColor3f(0, 1, 0);
    glVertex3f(p.x, 1.0, 0);
    glVertex3f(p.x + p.width, 1.0, 0);
    glVertex3f(p.x + p.width, -1.0 + p.height + PIPE_GAP_HEIGHT, 0);
    glVertex3f(p.x, -1.0 + p.height + PIPE_GAP_HEIGHT, 0);
    glEnd();
}

bool birbTouchesPipe()
{
    double rad = birb.width/900/2;
    bool touching = false;
    for(int i = 0; i < pipes.length(); i++)
    {
        if(birb.x/WINDOW_WIDTH+rad < pipes[i]->value.x)
            continue;
        if(birb.x/WINDOW_WIDTH-rad > pipes[i]->value.x + pipes[i]->value.width)
            continue;
        if(birb.y+rad < -1.0 + pipes[i]->value.height + PIPE_GAP_HEIGHT && birb.y-rad > -1.0 + pipes[i]->value.height)
            continue;
        touching = true;
    }
    return touching;
}

void updateBirb()
{
    if (birb.y <= (WINDOW_HEIGHT - birb.width) / -WINDOW_HEIGHT || birbTouchesPipe() || birb.y >= (WINDOW_HEIGHT - birb.width) / WINDOW_HEIGHT)
        birb.isDead = true;
    if (!birb.isDead)
    {
        birb.y += birb.velocity;
        birb.velocity -= GRAVITY;
    }
    glPushMatrix();
    glTranslatef(0, birb.y, 0);
    drawCircle(birb.x, birb.y, birb.width);
    glPopMatrix();
}

void reset()
{
    score = 0;
    birb.y = 0.0;
    birb.isDead = false;
    birb.velocity = 0.03;
    pipes.clear();
    int amountOfPipes = 5;
    for (int i = 0; i < amountOfPipes; i++)
    {
        Pipe p = Pipe();
        p.x = 0.8 * i;
        p.height = 0.4;
        pipes.append(p);
    }
}

Pipe getNextPipe()
{
    /* check for which one of the first three
     * pipes is the next */
    Pipe currentClosest = pipes[0]->value;
    for(int i = 0; i < pipes.length(); i++)
    {
        Pipe temp = pipes[i]->value;
        if (temp.x+temp.width < )
    }
}

/* glut functions */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw da birb */
    updateBirb();

    /* draw pipes */
    for (int i = 0; i < pipes.length(); i++)
    {
        if (!birb.isDead)
            pipes[i]->value.x -= PIPE_MOVEMENT_SPEED;
        if (pipes[i]->value.x + pipes[i]->value.width < -1.0)
        {
            score++;
            std::cout << score << std::endl;
            pipes[i]->value.x = pipes[-1]->value.x + 0.8;
            pipes.append(pipes.pop(0));

        }
        drawPipe(pipes[i]->value);
    }

    glutSwapBuffers();

    glFlush();
}

void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        /* !! have to match with initialization value !! */
        if (birb.isDead)
        {
            reset();
        }
        birb.velocity = 0.03;
    }
}

int main(int argc, char **argv)
{
    /* initialization of pipes */
    int amountOfPipes = 5;
    for (int i = 0; i < amountOfPipes; i++)
    {
        Pipe p = Pipe();
        p.x = 0.8 * i;
        p.height = 0.4;
        pipes.append(p);
    }

    /* initialisation */
    /* Window Coordinates are from -900 to +900 for both axis */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Fappy Birb");

    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}