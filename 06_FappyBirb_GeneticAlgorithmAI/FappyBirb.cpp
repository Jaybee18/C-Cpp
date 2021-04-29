/* compiling
 * linux   : g++ 06_FappyBirb_GeneticAlgorithmAI/FappyBirb.cpp -lglut -lGL -o 06_FappyBirb_GeneticAlgorithmAI/FappyBirb; ./06_FappyBirb_GeneticAlgorithmAI/FappyBirb
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
#include "GeneticAlgoritm.h"

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
static int forceDeathScore = 5000;
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
Pipe currentLastPipe;
list<Pipe> pipes;
vector<Agent> agents;
vector<Birb> birbs;

/* shapes and figures */
void drawCircle(int x, double y, int diameter)
{
    double center[2] = {x * 1.0, y * 1.0};
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 1);
    for (int i = 0; i < 360; i++)
    {
        double yDelta = diameter/2 * sin(i);
        double xDelta = diameter/2 * cos(i);
        glVertex3f((center[0] + xDelta) / WINDOW_WIDTH, (center[1] + yDelta) / WINDOW_HEIGHT, 0);
    }
    glEnd();
}

void drawPipe(Pipe p)
{
    /* lower half 
    glBegin(GL_POLYGON);
    glColor3f(0, 1, 0);
    glVertex3f(p.x, -1.0, 0);
    glVertex3f(p.x + p.width, -1.0, 0);
    glVertex3f(p.x + p.width, -1.0 + p.height, 0);
    glVertex3f(p.x, -1.0 + p.height, 0);
    glEnd();
    /* upper half 
    glBegin(GL_POLYGON);
    glColor3f(0, 1, 0);
    glVertex3f(p.x, 1.0, 0);
    glVertex3f(p.x + p.width, 1.0, 0);
    glVertex3f(p.x + p.width, -1.0 + p.height + PIPE_GAP_HEIGHT, 0);
    glVertex3f(p.x, -1.0 + p.height + PIPE_GAP_HEIGHT, 0);
    glEnd();*/
    
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
    /* check for which one of the first three
       pipes is the next  */
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

void updateBirb(Birb &b, Agent &a)
{
    if (b.y <= (WINDOW_HEIGHT - b.width) / -WINDOW_HEIGHT || birbTouchesPipe(b) || b.y >= (WINDOW_HEIGHT - b.width) / WINDOW_HEIGHT)
    {
        a.isDead = true;
        Pipe temp = getNextPipe(b);
    }
    if (!a.isDead)
    {
        b.y += b.velocity;
        b.velocity -= GRAVITY;
    }
    else
    {
        return;
    }
    if (generation % generationShow == 0 && showBirbs)
    {
        glPushMatrix();
        glTranslatef(0, b.y, 0);
        drawCircle(b.x, b.y, b.width);
        glPopMatrix();
    }
}

void reset(vector<Agent> newAgents)
{
    /* initialization of pipes */
    pipes.clear();
    for (int i = 0; i < amountOfPipes; i++)
    {
        Pipe p = Pipe();
        p.x = startPipeDistande + pipeDistance * i;
        p.height = rand() / (RAND_MAX * 1.0);
        pipes.append(p);
    }

    /* initialization of agents */
    agents.clear();
    birbs.clear();
    for (int i = 0; i < amountOfAgents; i++)
    {
        birbs.push_back(Birb());
    }
    agents = newAgents;
}

void reset()
{
    /* initialization of pipes */
    pipes.clear();
    for (int i = 0; i < amountOfPipes; i++)
    {
        Pipe p = Pipe();
        p.x = startPipeDistande + pipeDistance * i;
        p.height = rand() / (RAND_MAX * 1.0);
        pipes.append(p);
    }

    /* initialization of agents */
    agents.clear();
    birbs.clear();
    for (int i = 0; i < amountOfAgents; i++)
    {
        agents.push_back(Agent(3, agentNetworkTopology));
        birbs.push_back(Birb());
    }
}

void jump(Birb &b)
{
    b.velocity = 0.03;
}

/* glut functions */
void display()
{
    if (generation % generationShow == 0)
        glClear(GL_COLOR_BUFFER_BIT);

    /* draw pipes */
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

    /* let the agent play */
    bool allDead = true;
    Pipe temp = getNextPipe(birbs[0]);
    for (int index = 0; index < amountOfAgents; index++)
    {
        Agent *currentAgent = &agents[index];
        Birb *currentBirb = &birbs[index];

        if(currentAgent->isDead)
            continue;

        /* draw and move da brib */
        updateBirb(*currentBirb, *currentAgent);
        if (!currentAgent->isDead)
        {
            currentAgent->score++;
            if (currentAgent->score >= forceDeathScore)
                currentAgent->isDead = true;
            allDead = false;
        }else{continue;}

        /* draw lines to show what the
         * agents get as their input 
         */
        if (showBirbSight && generation % generationShow == 0)
        {
            drawOneLine(currentBirb->x / 900 - currentBirb->width / 900, currentBirb->y, temp.x + temp.width, currentBirb->y); // ditance to next pipe
            drawOneLine(temp.x + temp.width, currentBirb->y, temp.x + temp.width, -1.0 + temp.height + PIPE_GAP_HEIGHT / 2);   // y delta
        }

        /* all possible inputs for the agents are
         * defined here so that a hotswap is as
         * easy as possible
         */
        double distToNextPipe = temp.x + temp.width - currentBirb->x / 900 - currentBirb->width / 900 / 2;
        double yPosition = currentBirb->y - 1.0;
        double yOfNextHole = (currentBirb->y - -1.0 + temp.height + PIPE_GAP_HEIGHT);
        //double upperBound = temp.height + PIPE_GAP_HEIGHT;
        //double lowerBound = temp.height;
        //double yVel = currentBirb->velocity;
        //double xVel = PIPE_MOVEMENT_SPEED;

        /* the agent gets given the his inputs
         * and his output determines whether 
         * or not it jumps
         */
        vector<double> output = currentAgent->forwardPass({distToNextPipe, yOfNextHole, yPosition});
        if (output[0] > jumpingThreshold)
            jump(*currentBirb);
    }

    /* when all agents are dead
     * choose the best ones and
     * cross them
     */
    if (allDead)
    {
        /* 
         * select the best 5% of the last generation
         * to safely and without mutation pass into
         * the next generation, so that progress 
         * isn't lost 
         */
        vector<Agent> bestOfGen = getBest(agents, 0.05);

        std::cout << "Generation : " << generation << " || Score : " << bestOfGen[0].score << std::endl;
        generation++;

        /*
         * The best two agents are selected to create
         * a new gene from which all other agents 
         * for the next generation are generated.
         * During that generating process they
         * can mutate
         */
        vector<double> genes1 = extractWeights(bestOfGen[0]), genes2 = extractWeights(bestOfGen[1]);
        vector<double> newGene = generateNewGene(genes1, genes2, bestOfGen[0].score, bestOfGen[1].score);
        vector<Agent> newAgents = generateAgentsFromGene(amountOfAgents - bestOfGen.size(), newGene, agentNetworkTopology, agentInputs);

        /* add the best 5% of last generation to 
         * the new generation
         */
        for (Agent a : bestOfGen)
        {
            /* !! reset all the agents attributes !!*/
            /* after playing all the agents will have their
             * isDead attribute set on true. So without resetting
             * they will all just die at the beginning providing
             * no progress to the generation */
            a.isDead = false;
            a.score = 0;
            newAgents.push_back(a);
        }
        reset(newAgents);
    }

    if (generation % generationShow == 0)
    {
        glutSwapBuffers();
        glFlush();
    }
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

    /* initialization of agents */
    for (int i = 0; i < amountOfAgents; i++)
    {
        agents.push_back(Agent(agentInputs, agentNetworkTopology));
        birbs.push_back(Birb());
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

    glutMainLoop();
    return 0;
}
