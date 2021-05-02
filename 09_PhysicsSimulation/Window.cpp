#ifdef _WIN32
#include "C:\MinGW\include\GL\freeglut.h"
#else
#include "GL/freeglut.h"
#endif
#include "GL/gl.h"
#include <vector>

using std::vector;

/* general constants/variables */
int FPS = 60;
vector<Rigidbody> objects;

/* definition of objects */
class Rigidbody{
    protected:
        double x, y;
        double xvel, yvel;
    public:
        Rigidbody();
        void render();
        ~Rigidbody();
};

class _Rectangle : public Rigidbody{
    private:
        int width, height;
    public:
        _Rectangle(double x, double y, int width, int height){this->width = width; this->height = height;this->x = x; this->y = y;};
        void render(){
            glBegin(GL_LINE_LOOP);
            glColor3i(1, 0, 0);
            glVertex2d(x, y);
            glVertex2d(x+width, y);
            glVertex2d(x+width, y+height);
            glVertex2d(x, y+height);
            glEnd();
        };
        ~_Rectangle();
};

/* main class */
class Window
{
private:
    int WINDOW_WIDTH, WINDOW_HEIGHT;
public:
    Window(int argc, char **argv);
    ~Window();
};

void displayFunction(){
    glClear(GL_COLOR_BUFFER_BIT);
    for(Rigidbody *r : objects)
    {
        r->render();
    }
    glutSwapBuffers();
    glFlush();
}

void timer(int v)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

Window::Window(int argc, char **argv)
{
    WINDOW_WIDTH = 600;
    WINDOW_HEIGHT = 600;
    Rigidbody temp = _Rectangle(0.5, 0.5, 1, 1);
    objects.push_back(temp);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Fappy Birb");

    glutDisplayFunc(displayFunction);
    glutTimerFunc(100, timer, 0);

    glutMainLoop();
}

Window::~Window()
{
}
