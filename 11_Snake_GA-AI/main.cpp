// compiling linux : g++ 11_Snake_GA-AI/main.cpp -lglut -lGL -o 11_Snake_GA-AI/main; ./11_Snake_GA-AI/main

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
};

/* general constants/variables */
static int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 600;
static int FPS = 60;
static int squareSize = 60;
bool dead = false;
int currentFoodPos[2] = {0, 0};
int vel[2] = {1, 0};
//vector<list<body_part>> snakes;
list<body_part> snake;
Agent agent;

void replaceFood()
{
    int newX = randomDouble() * (WINDOW_WIDTH / squareSize);
    currentFoodPos[0] = newX * squareSize;
    int newY = randomDouble() * (WINDOW_HEIGHT / squareSize);
    currentFoodPos[1] = newY * squareSize;
}

void reset(){
    snake.clear();
    body_part temp = body_part();
    temp.x = 0;
    temp.y = 0;
    snake.append(temp);
    dead = false;
    agent = Agent(4, {6, 6, 4}); // TEMPORARY
    replaceFood();
}

void drawSquare(float x, float y, float width, float height)
{
    glBegin(GL_LINE_LOOP);
    glVertex3f(x / WINDOW_WIDTH, y / WINDOW_HEIGHT, 0);
    glVertex3f(x / WINDOW_WIDTH + width / WINDOW_WIDTH, y / WINDOW_HEIGHT, 0);
    glVertex3f(x / WINDOW_WIDTH + width / WINDOW_WIDTH, y / WINDOW_HEIGHT + height / WINDOW_HEIGHT, 0);
    glVertex3f(x / WINDOW_WIDTH, y / WINDOW_HEIGHT + height / WINDOW_HEIGHT, 0);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /* don't render if dead */
    if (dead)
        reset();

    /* draw the food */
    glColor3f(1, 0, 0);
    drawSquare(currentFoodPos[0], currentFoodPos[1], squareSize, squareSize);

    /* update the snake */
    // inputs
    body_part head = snake[0]->value;
    double leftFood = 0;
    if(currentFoodPos[1] == head.y && currentFoodPos[0] < head.x)
        leftFood = 1;
    double rightFood = 0;
    if(currentFoodPos[1] == head.y && currentFoodPos[0] > head.x)
        rightFood = 1;
    double aboveFood = 0;
    if(currentFoodPos[1] > head.y && currentFoodPos[0] == head.x)
        aboveFood = 1;
    double underFood = 0;
    if(currentFoodPos[1] < head.y && currentFoodPos[0] == head.x)
        underFood = 1;
    vector<double> output = agent.forwardPass({leftFood, rightFood, aboveFood, underFood});
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
        vel[0] = 0;
        vel[1] = 1;
        break;
    case 1:
        vel[0] = -1;
        vel[1] = 0;
        break;
    case 2:
        vel[0] = 0;
        vel[1] = -1;
        break;
    case 3:
        vel[0] = 1;
        vel[1] = 0;
        break;
    default:
        break;
    }
    // check if snake has eaten food
    if (snake[0]->value.x == currentFoodPos[0] && snake[0]->value.y == currentFoodPos[1])
    {
        body_part temp = body_part();
        temp.x = 0;
        temp.y = 0;
        snake.append(temp);
        replaceFood();
    }
    // update snakes positions
    for (int i = snake.length() - 1; i > 0; i--)
    {
        snake[i]->value = snake[i - 1]->value;
    }
    snake[0]->value.x += vel[0] * squareSize;
    snake[0]->value.y += vel[1] * squareSize;

    /*draw the snake */
    glColor3f(1, 1, 1);
    for (int body_index = 0; body_index < snake.length(); body_index++)
        drawSquare(snake[body_index]->value.x, snake[body_index]->value.y, squareSize, squareSize);

    /* check if snake is dead */
    body_part first = snake[0]->value;
    if (first.x < -WINDOW_WIDTH || first.x > WINDOW_WIDTH || first.y < -WINDOW_HEIGHT || first.y > WINDOW_HEIGHT)
        dead = true;
    for (int i = 1; i < snake.length(); i++)
    {
        if (first.x == snake[i]->value.x && first.y == snake[i]->value.y)
            dead = true;
    }

    glutSwapBuffers();
    glFlush();
}

void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        vel[0] = 0;
        vel[1] = 1;
        break;
    case 'a':
        vel[0] = -1;
        vel[1] = 0;
        break;
    case 's':
        vel[0] = 0;
        vel[1] = -1;
        break;
    case 'd':
        vel[0] = 1;
        vel[1] = 0;
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{
    /* enviroment initialization */
    replaceFood();
    body_part temp = body_part();
    temp.x = 0;
    temp.y = 0;
    snake.append(temp);
    agent = Agent(4, {6, 6, 4});

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Fappy Birb");

    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);
    //glutKeyboardFunc(keyboard);

    glutMainLoop();
}