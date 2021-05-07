// compiling linux : g++ 11_Snake_GA-AI/main.cpp -lglut -lGL -o 11_Snake_GA-AI/main; ./11_Snake_GA-AI/main
// compiling windows : g++ 11_Snake_GA-AI/main.cpp -o 11_Snake_GA-AI/main.exe -I"C:\MinGW\include" -L"C:\MinGW\lib" -lfreeglut -lopengl32 && 11_Snake_GA-AI\main.exe

#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "GL/freeglut.h"
#endif
#include "GL/gl.h"
#include <math.h>
#include <iostream>
#include "../_GeneralHeaders/plist.h"
#include "../_GeneralHeaders/NEAT.h"

/* objects */
struct body_part
{
    int x, y;
    int xvel, yvel;
};

/* general constants/variables */
static int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 600;
static int FPS = 1;
static int squareSize = 60;
static int forceDeathScore = 100;
static int forceDeathFrames = 100;
static vector<int> networkTopology = {6, 6, 4};
static int amountOfAgents = 1;
static int agentInputs = 4;
int frames = 0;
vector<vector<int>> foods;
vector<list<body_part>> snakes;
NEAT neat;

/* game control methods */
void replaceFood(vector<int> &food)
{
    int newX = randomDouble() * (WINDOW_WIDTH / squareSize);
    food[0] = newX * squareSize;
    int newY = randomDouble() * (WINDOW_HEIGHT / squareSize);
    food[1] = newY * squareSize;
}
void reset()
{
    std::cout << "penis resert" << std::endl;
    frames = 0;
    snakes.clear();
    body_part temp = body_part();
    temp.x = 0;
    temp.y = 0;
    temp.xvel = 1;
    temp.yvel = 0;
    list<body_part> snake;
    snake.append(temp);
    for (int i = 0; i < amountOfAgents; i++)
    {
        snakes.push_back(snake);
        replaceFood(foods[i]);
    }
    neat.generateNewGeneration();
}
void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

/* drawing methods */
void drawSquare(float x, float y, float width, float height)
{
    glBegin(GL_LINE_LOOP);
    glVertex3f(x / WINDOW_WIDTH, y / WINDOW_HEIGHT, 0);
    glVertex3f(x / WINDOW_WIDTH + width / WINDOW_WIDTH, y / WINDOW_HEIGHT, 0);
    glVertex3f(x / WINDOW_WIDTH + width / WINDOW_WIDTH, y / WINDOW_HEIGHT + height / WINDOW_HEIGHT, 0);
    glVertex3f(x / WINDOW_WIDTH, y / WINDOW_HEIGHT + height / WINDOW_HEIGHT, 0);
    glEnd();
}
void drawSnake(list<body_part> pSnake)
{
    glColor3f(1, 1, 1);
    for (int body_index = 0; body_index < pSnake.length(); body_index++)
        drawSquare(pSnake[body_index]->value.x, pSnake[body_index]->value.y, squareSize, squareSize);
}

/* update methods */
void drawFood(vector<int> food)
{
    glColor3f(1, 0, 0);
    drawSquare(food[0], food[1], squareSize, squareSize);
}
void updateSnake(list<body_part> &pSnake, vector<int> &food, int index)
{
    // inputs
    body_part head = pSnake[0]->value;
    double leftFood = 0;
    if (food[1] == head.y && food[0] < head.x)
        leftFood = 1;
    double rightFood = 0;
    if (food[1] == head.y && food[0] > head.x)
        rightFood = 1;
    double aboveFood = 0;
    if (food[1] > head.y && food[0] == head.x)
        aboveFood = 1;
    double underFood = 0;
    if (food[1] < head.y && food[0] == head.x)
        underFood = 1;
    vector<double> output = neat.getSingleAgentOutput(index, {leftFood, rightFood, aboveFood, underFood});
    int indexOfLargest = 0;
    double valueOfLargest = 0.0;
    for (int i = 0; i < output.size(); i++)
    {
        if (output[i] > valueOfLargest)
        {
            indexOfLargest = i;
            valueOfLargest = output[i];
        }
    }
    switch (indexOfLargest)
    {
    case 0:
        snakes[index][0]->value.xvel = 0;
        snakes[index][0]->value.yvel = 1;
        break;
    case 1:
        snakes[index][0]->value.xvel = -1;
        snakes[index][0]->value.yvel = 0;
        break;
    case 2:
        snakes[index][0]->value.xvel = 0;
        snakes[index][0]->value.yvel = -1;
        break;
    case 3:
        snakes[index][0]->value.xvel = 1;
        snakes[index][0]->value.yvel = 0;
        break;
    default:
        break;
    }
    // check if snake has eaten food
    if (head.x == food[0] && head.y == food[1])
    {
        body_part temp = body_part();
        temp.x = 0;
        temp.y = 0;
        temp.xvel = 1;
        temp.yvel = 0;
        pSnake.append(temp);
        neat.increaseScore(index);
        replaceFood(food);
    }
    // update snakes positions
    for (int i = pSnake.length() - 1; i > 0; i--)
    {
        pSnake[i]->value = pSnake[i - 1]->value;
    }
    std::cout << snakes[index][0]->value.x << std::endl;
    snakes[index][0]->value.x += snakes[index][0]->value.xvel * squareSize;
    snakes[index][0]->value.y += snakes[index][0]->value.yvel * squareSize;

    std::cout << snakes[index][0]->value.x << std::endl;

    /* check if snake is dead */
    //body_part first = *head;
    //if (first.x < -WINDOW_WIDTH || first.x > WINDOW_WIDTH || first.y < -WINDOW_HEIGHT || first.y > WINDOW_HEIGHT)
    //    neat.killAgent(index);
    /*for (int i = 1; i < pSnake.length(); i++)
    {
        if (first.x == pSnake[i]->value.x && first.y == pSnake[i]->value.y)
            neat.killAgent(index);
    }*/
    //head = NULL;
}

/* main method */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (false/*neat.allDead()/* || frames >= forceDeathFrames*/)
        reset();

    for (int agentIndex = 0; agentIndex < amountOfAgents; agentIndex++)
    {
        if (neat.isAgentDead(agentIndex))
            continue;

        /* update the snake */
        updateSnake(snakes[agentIndex], foods[agentIndex], agentIndex);

        /* draw the food */
        drawFood(foods[agentIndex]);

        /*draw the snake */
        drawSnake(snakes[agentIndex]);
    }

    //frames++;
    glutSwapBuffers();
    glFlush();
}

int main(int argc, char **argv)
{
    /* enviroment initialization */
    neat = NEAT(forceDeathScore);
    neat.initializeAgents(amountOfAgents, agentInputs, networkTopology);
    list<body_part> snake;
    body_part temp = body_part();
    temp.x = 0;
    temp.y = 0;
    temp.xvel = 1;
    temp.yvel = 0;
    snake.append(temp);
    for (int i = 0; i < amountOfAgents; i++)
    {
        snakes.push_back(snake);
        foods.push_back({0, 0});
        replaceFood(foods.back());
    }

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