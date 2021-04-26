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
//#include "NeuralNetwork.h" // has to be removed bc of GeneticAlgoritm.h
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
    bool isDead = false;
    double width = 30.0;
    int score = 0;
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
static vector<int> agentNetworkTopology = {6, 6, 1};
static double PIPE_MOVEMENT_SPEED = 0.008;
static bool showBirbSight = true;
static int amountOfAgents = 200;
static double GRAVITY = 0.0015;
static int forceDeathScore = 5000;
static int pipeDistance = 1;
static int startPipeDistande = 10.0;
static double jumpingThreshold = 0.65;
static int amountOfPipes = 7;
static int generationShow = 1;
static int agentInputs = 4;
static int FPS = 500;
int bestScore = 0;
int generation = 0;
bool movePipes = true;
double distToNextPipe = 0.0;
double yPosittion = 0.0;
double yOfNextHole = 0.0;
vector<double> lastBestGene;

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
        if (temp.x < currentClosest.x && temp.x > b.x / 900 - b.width / 900)
            return temp;
    }
    return currentClosest;
}

void updateBirb(Birb &b)
{
    if (b.y <= (WINDOW_HEIGHT - b.width) / -WINDOW_HEIGHT || birbTouchesPipe(b) || b.y >= (WINDOW_HEIGHT - b.width) / WINDOW_HEIGHT)
    {
        b.isDead = true;
        Pipe temp = getNextPipe(b);
        //b.score = b.score / (1 + abs(b.y - (-1.0 + temp.height + PIPE_GAP_HEIGHT / 2)));
    }
    if (!b.isDead)
    {
        b.y += b.velocity;
        b.velocity -= GRAVITY;
    }
    else
    {
        return;
    }
    if (generation % generationShow == 0)
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
    if(generation % generationShow == 0)
        glClear(GL_COLOR_BUFFER_BIT);

    /* draw da birb */
    for (Birb &b : birbs)
        updateBirb(b);
    for (int i = 0; i < amountOfAgents; i++)
        if (!birbs[i].isDead)
        {
            birbs[i].score++;
            if (birbs[i].score >= forceDeathScore)
            {
                birbs[i].isDead = true;
            }
        }

    /* draw pipes */
    for (int i = 0; i < pipes.length(); i++)
    {
        if(movePipes)
            pipes[i]->value.x -= PIPE_MOVEMENT_SPEED;
        if (pipes[i]->value.x + pipes[i]->value.width < -1.0)
        {
            /*for (int j = 0; j < amountOfAgents; j++)
                if (!birbs[j].isDead)
                {
                    //scores[j]++;
                    //if (scores[j] >= deathMaxScore)
                    //    birbs[j].isDead = true;
                }*/
            pipes[i]->value.x = pipes[-1]->value.x + pipeDistance;
            pipes[i]->value.height = rand() / (RAND_MAX * 1.0);
            pipes.append(pipes.pop(0));
        }
        if (movePipes && generation % generationShow == 0)
            drawPipe(pipes[i]->value);
    }

    /* draw birb sight */
    // todo : maybe move this into another programm
    //        since this should only be the base game
    /*if (showBirbSight)
    {
        Pipe temp = getNextPipe(birbs[0]);
        //drawOneLine(birb.x / 900, birb.y, temp.x, -1.0 + temp.height + PIPE_GAP_HEIGHT / 2);
        drawOneLine(birbs[0].x / 900, birbs[0].y, temp.x + temp.width, birbs[0].y);
        drawOneLine(temp.x + temp.width, birbs[0].y, temp.x + temp.width, -1.0 + temp.height + PIPE_GAP_HEIGHT / 2);
        drawOneLine(birbs[0].x / 900, birbs[0].y, birbs[0].x / 900, -1.0);
    }*/

    /* let the agent play */
    bool allDead = true;
    for (int index = 0; index < amountOfAgents; index++)
    {
        if(birbs[index].isDead)
            continue;
        Pipe temp = getNextPipe(birbs[index]);
        if (showBirbSight && generation % generationShow == 0)
        {
            drawOneLine(birbs[index].x / 900, birbs[index].y, temp.x + temp.width, birbs[index].y); // ditance to next pipe
            drawOneLine(temp.x + temp.width, birbs[index].y, temp.x + temp.width, -1.0 + temp.height + PIPE_GAP_HEIGHT / 2); // y delta
            //drawOneLine(birbs[index].x / 900, birbs[index].y, birbs[index].x / 900, -1.0); // distance to ground
        }
        if (!birbs[index].isDead)
            allDead = false;
        distToNextPipe = temp.x + temp.width - birbs[index].x / 900;
        yPosittion = birbs[index].y + 1;
        yOfNextHole = (birbs[index].y - -1.0 + temp.height + PIPE_GAP_HEIGHT);
        double upperBound = temp.height + PIPE_GAP_HEIGHT;
        double lowerBound = temp.height;
        double yVel = birbs[index].velocity;
        double xVel = PIPE_MOVEMENT_SPEED;
        vector<double> output = agents[index].forwardPass({distToNextPipe, yOfNextHole, yVel, xVel});
        if (output[0] > jumpingThreshold)
            jump(birbs[index]);
    }

    /* when all agents are dead
     * choose the best ones and
     * cross them
     */
    if (allDead)
    {
        int maxScore = 0, maxSecondScore = 0;
        Agent first = agents[0], second = agents[1];
        for (int i = 0; i < birbs.size(); i++)
        {
            if (birbs[i].score > maxScore)
            {
                maxScore = birbs[i].score;
                first = agents[i];
            }
            else if (birbs[i].score > maxSecondScore)
            {
                maxSecondScore = birbs[i].score;
                second = agents[i];
            }
        }
        std::cout << "Generation : " << generation << " || Score : " << maxScore << " || Total Max Score : " << bestScore << std::endl;
        generation++;
        vector<double> genes1 = extractWeights(first), genes2 = extractWeights(second);
        vector<double> newGene = generateNewGene(genes1, genes2, maxScore, maxSecondScore);
        vector<Agent> newAgents = generateAgentsFromGene(amountOfAgents, newGene, agentNetworkTopology);

        // todo : something here is maybe weird !!!!
        if (maxScore >= bestScore)
        {
            reset(newAgents);
            bestScore = maxScore; // experimental
            lastBestGene = genes1;
        }
        else
        {
            reset(generateAgentsFromGene(amountOfAgents, lastBestGene, agentNetworkTopology));
        }
    }

    if(generation % generationShow == 0){
    glutSwapBuffers();
    glFlush();}
}

void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

int main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));
    //for(int i = 0; i < 100; i++)
    //    std::cout << rand()/(RAND_MAX*1.0) << std::endl;

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
    //glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
