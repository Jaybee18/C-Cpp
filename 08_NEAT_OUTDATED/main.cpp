/* compiling
 * linux   : g++ 08_NEAT/main.cpp -lglut -lGL -o 08_NEAT/main; ./08_NEAT/main
 * windows : g++ 08_NEAT/main.cpp -o 08_NEAT/main.exe -I"C:\MinGW\include" -L"C:\MinGW\lib" -lfreeglut -lopengl32 && 08_NEAT\main.exe
 */

#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "GL/freeglut.h"
#endif
#include "GL/gl.h"
#include <math.h>
#include <iostream>
#include "../_GeneralHeaders/plist.h"
#include "headers/NEAT.h"

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
    double velocity = 0.00;
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
static double PIPE_GAP_HEIGHT = 600.0 / WINDOW_HEIGHT;
static vector<int> agentNetworkTopology = {6, 1};
static double PIPE_MOVEMENT_SPEED = 0.008;
static bool showBirbSight = false;
static bool showBirbs = true;
static int amountOfAgents = 700;
static double GRAVITY = 0.0015;
static int forceDeathScore = 3000;
static int pipeDistance = 1;
static int startPipeDistande = 1.8;
static double jumpingThreshold = 0.6;
static int amountOfPipes = 4;
static int generationShow = 1;
static int agentInputs = 3;
static int FPS = 500;
int bestScore = 0;
int generation = 0;
bool movePipes = true;

/* objects */
list<Pipe> pipes;
vector<Birb> birbs;
NEAT neat;

/* shapes and figures */
void drawCircle(int x, double y, int diameter)
{
    double center[2] = {x * 1.0, y * 1.0};
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 1);
    for (int i = 0; i < 360; i++)
    {
        double yDelta = diameter / 2 * sin(i);
        double xDelta = diameter / 2 * cos(i);
        glVertex3f((center[0] + xDelta) / WINDOW_WIDTH, (center[1] + yDelta) / WINDOW_HEIGHT, 0);
    }
    glEnd();
}

void drawPipe(Pipe p)
{
    /* lower half */
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 0);
    glVertex3f(p.x, -1.0, 0);
    glVertex3f(p.x + p.width, -1.0, 0);
    glVertex3f(p.x + p.width, -1.0 + p.height, 0);
    glVertex3f(p.x, -1.0 + p.height, 0);
    glEnd();

    /* upper half */
    glBegin(GL_LINE_LOOP);
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

Pipe getNextPipe(Birb b)
{

    Pipe currentClosest = pipes[-1]->value;
    for (int i = 0; i < pipes.length(); i++)
    {
        Pipe temp = pipes[i]->value;
        //if (temp.x + temp.width < birb.x - birb.width / 900 / 2)
        //    continue;
        if (temp.x < currentClosest.x && temp.x + temp.width > b.x / 900 - b.width / 900 / 2)
            return temp;
    }
    return currentClosest;
}

void updateBirb(int birbIndex)
{
    Birb *b = &birbs[birbIndex];
    if (b->y <= (WINDOW_HEIGHT - b->width) / -WINDOW_HEIGHT || birbTouchesPipe(*b) || b->y >= (WINDOW_HEIGHT - b->width) / WINDOW_HEIGHT)
    {
        neat.killAgent(birbIndex);
        return;
    }
    b->y += b->velocity;
    b->velocity -= GRAVITY;
    if (generation % generationShow == 0 && showBirbs)
    {
        glPushMatrix();
        glTranslatef(0, b->y, 0);
        drawCircle(b->x, b->y, b->width);
        glPopMatrix();
    }
}

void jump(Birb &b)
{
    b.velocity = 0.03;
}

/* glut functions */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /* update and draw pipes */
    for (int i = 0; i < pipes.length(); i++)
    {
        if (movePipes)
            pipes[i]->value.x -= PIPE_MOVEMENT_SPEED;
        if (pipes[i]->value.x + pipes[i]->value.width < -1.0)
        {
            pipes[i]->value.x = pipes[-1]->value.x + pipeDistance;
            pipes[i]->value.height = rand() / (RAND_MAX * 1.0);
            pipes.append(pipes.pop(0));
        }
        if (movePipes && generation % generationShow == 0)
            drawPipe(pipes[i]->value);
    }
    
    Pipe nexPipe = getNextPipe(birbs[0]);
    for (int i = 0; i < birbs.size(); i++)
    {
        if (neat.isAgentDead(i))
            continue;
        
        /* give the birb his inputs and let him make his move */
        double distToNextPipe = nexPipe.x + nexPipe.width - birbs[i].x / 900 - birbs[i].width / 900 / 2;
        double yPosition = birbs[i].y - 1.0;
        double yOfNextHole = (birbs[i].y - -1.0 + nexPipe.height + PIPE_GAP_HEIGHT);
        double out = neat.getSingleAgentOutput(i, {distToNextPipe, yPosition, yOfNextHole})[0];

        /* jump if the output is high enough */
        if (out > jumpingThreshold)
            jump(birbs[i]);
        
        /* update and draw the birb */
        updateBirb(i);
        neat.increaseScore(i);
    }

    if (neat.allDead())
    {
        /* generate a new generation of agents */
        neat.generateNewGeneration();

        /* reset the birbs and the pipes positions */
        for (int i = 0; i < amountOfPipes; i++)
        {
            Pipe *p = &(pipes[i]->value);
            p->x = startPipeDistande + pipeDistance * i;
            p->height = rand() / (RAND_MAX * 1.0);
        }
        for (int i = 0; i < amountOfAgents; i++)
        {
            birbs[i].y = 0.0;
            birbs[i].velocity = 0.0;
        }
    }

    glutSwapBuffers();
    glFlush();
}

void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

int main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));

    /* initialization of pipes */
    for (int i = 0; i < amountOfPipes; i++)
    {
        Pipe p = Pipe();
        p.x = startPipeDistande + pipeDistance * i;
        p.height = rand() / (RAND_MAX * 1.0);
        pipes.append(p);
    }

    /* initialization of birbs */
    for(int i = 0; i < amountOfAgents; i++){
        birbs.push_back(Birb());
    }

    neat = NEAT(forceDeathScore);
    neat.initializeAgents(amountOfAgents, agentInputs, agentNetworkTopology);
    
    /* initialisation */
    /* Window Coordinates are from -900 to +900 for both axis */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Fappy Birb");

    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);

    glutMainLoop();
    return 0;
}
