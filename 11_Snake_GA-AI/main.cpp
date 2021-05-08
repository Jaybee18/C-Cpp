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
static int FPS = 100;
static int squareSize = 50;
static int forceDeathScore = 100;
static int maxStepsWithoutFood = 300;
static vector<int> networkTopology = {16, 8, 4};
static int amountOfAgents = 1000;
static int agentInputs = 8;
vector<int> stepsSinceLastFood;
vector<vector<int>> foods;
vector<vector<body_part>> snakes;
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
    snakes.clear();
    body_part temp = body_part();
    temp.x = 0;
    temp.y = 0;
    temp.xvel = 1;
    temp.yvel = 0;
    vector<body_part> snake;
    snake.push_back(temp);
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
void keyboard(unsigned char key, int x, int y){
    if(key == 'k')
        for(int i = 0; i < amountOfAgents; i++)
            neat.killAgent(i);
}
bool bodyNearHead(vector<body_part> snake, vector<int> steps){
    body_part head = snake[0];
    vector<int> checkingPos = {head.x + steps[0]*squareSize, head.y + steps[1]*squareSize};
    for(body_part b : snake)
        if(b.x == checkingPos[0] && b.y == checkingPos[1])
            return true;
    return false;
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
void drawSnake(vector<body_part> snake)
{
    glColor3f(1, 1, 1);
    for (int body_index = 0; body_index < snake.size(); body_index++)
        drawSquare(snake[body_index].x, snake[body_index].y, squareSize, squareSize);
}

/* update methods */
void drawFood(vector<int> food)
{
    glColor3f(1, 0, 0);
    drawSquare(food[0], food[1], squareSize, squareSize);
}
void updateSnake(vector<body_part> &pSnake, vector<int> &food, int index)
{
    // inputs
    body_part *head = &pSnake[0];
    double leftFood = food[1] == head->y && food[0]  < head->x ? 1 : 0;
    double rightFood =food[1] == head->y && food[0]  > head->x ? 1 : 0;
    double aboveFood = food[1] > head->y && food[0] == head->x ? 1 : 0;
    double underFood = food[1] < head->y && food[0] == head->x ? 1 : 0;
    /* there is a wall next to the snake if 
    * a : it has reached the end of the screen
    * b : there is a body-part of the snake
    * c : it was on that square in the last frame (to prevent "jiggle-ing")
    */
    double leftWall = head->x == -WINDOW_WIDTH || bodyNearHead(pSnake, {-1, 0}) || head->xvel == 1 ? 1 : 0;
    double rightWall = head->x == WINDOW_WIDTH-squareSize || bodyNearHead(pSnake, {1, 0}) || head->xvel == -1 ? 1 : 0;
    double topWall = head->y == WINDOW_HEIGHT - squareSize || bodyNearHead(pSnake, {0, 1}) || head->yvel == -1 ? 1 : 0;
    double underWall = head->y == -WINDOW_HEIGHT || bodyNearHead(pSnake, {0, -1}) || head->yvel == 1 ? 1 : 0;
    vector<double> output = neat.getSingleAgentOutput(index, {leftFood, rightFood, aboveFood, underFood, leftWall, rightWall, topWall, underWall});
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
        /* up */
        if (head->yvel == -1)
            neat.killAgent(index);
        head->xvel = 0;
        head->yvel = 1;
        break;
    case 1:
        /* left */
        if (head->xvel == 1)
            neat.killAgent(index);
        head->xvel = -1;
        head->yvel = 0;
        break;
    case 2:
        /* down */
        if (head->yvel == 1)
            neat.killAgent(index);
        head->xvel = 0;
        head->yvel = -1;
        break;
    case 3:
        /* right */
        if (head->xvel == -1)
            neat.killAgent(index);
        head->xvel = 1;
        head->yvel = 0;
        break;
    default:
        break;
    }
    // check if snake has eaten food
    if (head->x == food[0] && head->y == food[1])
    {
        body_part temp = body_part();
        temp.x = 0;
        temp.y = 0;
        temp.xvel = 1;
        temp.yvel = 0;
        pSnake.push_back(temp);
        neat.increaseScore(index);
        replaceFood(food);
        stepsSinceLastFood[index] = 0;
    }else{stepsSinceLastFood[index]++;}
    // update snakes positions
    for (int i = pSnake.size() - 1; i > 0; i--)
    {
        pSnake[i] = pSnake[i - 1];
    }
    head->x += head->xvel * squareSize;
    head->y += head->yvel * squareSize;

    /* check if snake is dead */
    if (head->x < -WINDOW_WIDTH || head->x > WINDOW_WIDTH || head->y < -WINDOW_HEIGHT || head->y > WINDOW_HEIGHT)
        neat.killAgent(index);
    for (int i = 1; i < pSnake.size(); i++)
    {
        if (head->x == pSnake[i].x && head->y == pSnake[i].y)
            neat.killAgent(index);
    }
    if(stepsSinceLastFood[index] >= maxStepsWithoutFood)
        neat.killAgent(index);
}

/* main method */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (neat.allDead())
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

    glutSwapBuffers();
    glFlush();
}

int main(int argc, char **argv)
{
    /* enviroment initialization */
    neat = NEAT(forceDeathScore, 0.95);  // !important! : mutationrate is a very impactfull parameter!!!
    neat.initializeAgents(amountOfAgents, agentInputs, networkTopology);
    vector<body_part> snake;
    body_part temp = body_part();
    temp.x = 0;
    temp.y = 0;
    temp.xvel = 1;
    temp.yvel = 0;
    snake.push_back(temp);
    for (int i = 0; i < amountOfAgents; i++)
    {
        snakes.push_back(snake);
        foods.push_back({0, 0});
        stepsSinceLastFood.push_back(0);
        replaceFood(foods.back());
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Fappy Birb");

    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}