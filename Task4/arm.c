#include <stdlib.h>
#include<glut.h>

 static int elbow = 0.0, wrist = 0.0, fingers = 0.0;
 void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}
 void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
     glPushMatrix();
     // First Cube -- START
    glTranslatef(-4.0, -3.0, -3.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.2, 0.5);
    glColor3ub(255, 192, 203);
    glutWireCube(1.0);
    glPopMatrix();
    // First Cube -- END
     // Second Cube -- START
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(elbow, 0.0, 0.0, 0.8);
    glTranslatef(2.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(4.0, 0.2, 0.5);
    glColor3ub(255, 182, 193);
    glutWireCube(1.0);
    glPopMatrix();
    // Second Cube -- END
     // Finger 1 Part 1 - START
    glTranslatef(2.0, 0.3, 0.2);
    glRotatef(wrist, 0.0, 0.0, 1.0);
    glTranslatef(0.75, 0.0, 0.0);
    glPushMatrix();
    glScalef(1.5, 0.1, 0.1);
    glColor3ub(255, 105, 180);
    glutWireCube(1.0);
    glPopMatrix();
    // Finger 1 Part 1 - END
     // Finger 1 Part 2 - START
    glTranslatef(0.75, 0.0, 0.0);
    glRotatef(fingers, 0.0, 0.0, 1.0);
    glTranslatef(0.75, 0.0, 0.0);
    glPushMatrix();
    glScalef(1.5, 0.1, 0.1);
    glColor3ub(255, 105, 180);
    glutWireCube(1.0);
    glPopMatrix();
    // Finger 1 Part 2 - END
     glPopMatrix();
    glutSwapBuffers();
}
 void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}
 void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case '1':
            elbow = (elbow + 5) % 360;
            glutPostRedisplay();
            break;
        case '!':
            elbow = (elbow - 5) % 360;
            glutPostRedisplay();
            break;
        case '2':
            wrist = (wrist + 5) % 360;
            glutPostRedisplay();
            break;
        case '@':
            wrist = (wrist - 5) % 360;
            glutPostRedisplay();
            break;
        case '3':
            fingers = (fingers + 5) % 360;
            glutPostRedisplay();
            break;
        case '#':
            fingers = (fingers - 5) % 360;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}
 int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}