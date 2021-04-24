/* compiling
 * linux   : g++ 05_opengl/FappyBirb.cpp -lglut -lGL -o 05_opengl/FappyBirb; ./05_opengl/FappyBirb
 * windows : g++ 06_FappyBirb_GeneticAlgorithmAI\FappyBirb.cpp -o 06_FappyBirb_GeneticAlgorithmAI\FappyBirb.exe -I"C:\MinGW\include" -L"C:\MinGW\lib" -lfreeglut -lopengl32 && 06_FappyBirb_GeneticAlgorithmAI\FappyBirb.exe
 */

#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "GL/freeglut.h"
#endif
#include "GL/gl.h"
#include <math.h>
#include <iostream>
#include "plist.h"
#include "NeuralNetwork.h"

#define drawOneLine(x1, y1, x2, y2) \
    glBegin(GL_LINES);              \
    glColor3f(1, 0, 0);             \
    glVertex3f((x1), (y1), 0);      \
    glVertex3f((x2), (y2), 0);      \
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
static double PIPE_GAP_HEIGHT = 800.0 / WINDOW_HEIGHT;
static double PIPE_MOVEMENT_SPEED = 0.008;
static bool showBirbSight = true;
static int amountOfAgents = 100;
static double GRAVITY = 0.0015;
static int FPS = 120;
double distToNextPipe = 0.0;
double yPosittion = 0.0;
double yOfNextHole = 0.0;
vector<int> scores;

/* objects */
Pipe currentLastPipe;
list<Pipe> pipes;
//Birb birb = Birb();
vector<Agent> agents; // = Agent(3, {4, 4, 1});
vector<Birb> birbs;

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

void drawPipe(Pipe p)
{
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

bool birbTouchesPipe(Birb b)
{
    double rad = b.width / 900 / 2;
    bool touching = false;
    for (int i = 0; i < pipes.length(); i++)
    {
        if (b.x / WINDOW_WIDTH + rad < pipes[i]->value.x)
            continue;
        if (b.x / WINDOW_WIDTH - rad > pipes[i]->value.x + pipes[i]->value.width)
            continue;
        if (b.y + rad < -1.0 + pipes[i]->value.height + PIPE_GAP_HEIGHT && b.y - rad > -1.0 + pipes[i]->value.height)
            continue;
        touching = true;
    }
    return touching;
}

void updateBirb(Birb &b)
{
    if (b.y <= (WINDOW_HEIGHT - b.width) / -WINDOW_HEIGHT || birbTouchesPipe(b) || b.y >= (WINDOW_HEIGHT - b.width) / WINDOW_HEIGHT)
        b.isDead = true;
    if (!b.isDead)
    {
        b.y += b.velocity;
        b.velocity -= GRAVITY;
    }
    glPushMatrix();
    glTranslatef(0, b.y, 0);
    drawCircle(b.x, b.y, b.width);
    glPopMatrix();
}

/*double randomDouble()
{
    // yoinked from da net
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX + 1);
}*/

/*void reset()
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
        p.height = randomDouble()+1;
        pipes.append(p);
    }
}*/

Pipe getNextPipe(Birb b)
{
    /* check for which one of the first three
       pipes is the next  */
    Pipe currentClosest = pipes[-1]->value;
    for (int i = 0; i < pipes.length(); i++)
    {
        Pipe temp = pipes[i]->value;
        //if (temp.x + temp.width < birb.x - birb.width / 900 / 2)
        //    continue;
        if (temp.x < currentClosest.x && temp.x > b.x / 900 - b.width / 900 - 0.2)
            return temp;
    }
    return currentClosest;
}

void jump(Birb &b)
{
    b.velocity = 0.03;
}

/* glut functions */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw da birb */
    for (Birb &b : birbs)
        updateBirb(b);

    /* draw pipes */
    for (int i = 0; i < pipes.length(); i++)
    {
        pipes[i]->value.x -= PIPE_MOVEMENT_SPEED;
        if (pipes[i]->value.x + pipes[i]->value.width < -1.0)
        {
            for (int j = 0; j < amountOfAgents; j++)
                if (!birbs[j].isDead)
                    scores[j]++;
            pipes[i]->value.x = pipes[-1]->value.x + 0.8;
            pipes.append(pipes.pop(0));
        }
        drawPipe(pipes[i]->value);
    }

    Pipe temp;
    if (showBirbSight)
    {
        /* draw birb sight */
        // todo : maybe move this into another programm
        //        since this should only be the base game
        temp = getNextPipe(birbs[0]);
        //drawOneLine(birb.x / 900, birb.y, temp.x, -1.0 + temp.height + PIPE_GAP_HEIGHT / 2);
        drawOneLine(birbs[0].x / 900, birbs[0].y, temp.x, birbs[0].y);
        drawOneLine(temp.x, birbs[0].y, temp.x, -1.0 + temp.height + PIPE_GAP_HEIGHT / 2);
        drawOneLine(birbs[0].x / 900, birbs[0].y, birbs[0].x / 900, -1.0);
    }

    /* let the agent play */
    for (int index = 0; index < amountOfAgents; index++)
    {
        distToNextPipe = temp.x - birbs[index].x / 900;
        yPosittion = birbs[index].y;
        yOfNextHole = -1.0 + temp.height + PIPE_GAP_HEIGHT;
        vector<double> output = agents[index].forwardPass({distToNextPipe, yPosittion, yOfNextHole});
        if (output[0] > 0.5)
            jump(birbs[index]);
    }

    glutSwapBuffers();
    glFlush();
}

void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

/*void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        /* !! have to match with initialization value !! 
        if (birb.isDead)
        {
            reset();
        }
        jump();
    }
}*/

int main(int argc, char **argv)
{
    srand((unsigned) time(0));

    /* initialization of pipes */
    int amountOfPipes = 5;
    for (int i = 0; i < amountOfPipes; i++)
    {
        Pipe p = Pipe();
        p.x = 0.8 * i;
        p.height = rand()/(RAND_MAX*1.0);
        pipes.append(p);
    }

    /* initialization of agents */
    for (int i = 0; i < amountOfAgents; i++)
    {
        agents.push_back(Agent(3, {4, 4, 1}));
        birbs.push_back(Birb());
        scores.push_back(0);
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
    //glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
