#ifdef _WIN32
    #include "C:\MinGW\include\GL\freeglut.h"
#else
    #include "gl/freeglut.h"
#endif
#include "GL/gl.h"

/* compiling
 * linux   : g++ 05_opengl/main.cpp -lglut -lGL -o 05_opengl/main; ./05_opengl/main
 * windows : g++ 05_opengl/main.cpp -o 05_opengl/main.exe -I"C:\MinGW\include" -L"C:\MinGW\lib" -lfreeglut -lopengl32 && 05_opengl\main.exe
 */

void drawTriangle()
{
    glClearColor(0.4, 0.4, 0.4, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        glBegin(GL_TRIANGLES);
                glVertex3f(-0.7, 0.7, 0);
                glVertex3f(0.7, 0.7, 0);
                glVertex3f(0, -1, 1);
        glEnd();

    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(800, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Tetris");
    glutDisplayFunc(drawTriangle);
    glutMainLoop();
    return 0;
}