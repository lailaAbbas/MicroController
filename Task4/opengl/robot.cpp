#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SerialPort.h"
#include <math.h>
#include <glut.h>


/*
    Portname must contain these backslashes,
    and remember to replace the following com port
*/
char *port_name = "\\\\.\\COM7";

SerialPort arduino(port_name);
// String for incoming data
char incomingData[MAX_DATA_LENGTH];

bool top_finger_1_closing = false;
bool top_finger_1_opening = false;
bool top_finger_2_closing = false;
bool top_finger_2_opening = false;
int top_finger_1_joint_1 = 0.0;
int top_finger_1_joint_2 = 0.0;
int top_finger_1_joint_3 = 0.0;
int top_finger_2_joint_1 = 0.0;
int top_finger_2_joint_2 = 0.0;
int top_finger_2_joint_3 = 0.0;
int showGrid = 0;

double eye[] = {0,0,1};
double center[] = {0,0,0};
double up[] = {0,1,0};

void crossProduct(double a[], double b[], double c[])
{
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(double a[])
{
    double norm;
    norm = a[0]*a[0] + a[1]*a[1] + a[2]*a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{
    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2]*p[1] + a[1]*p[2];
    temp[1] = a[2]*p[0] - a[0]*p[2];
    temp[2] = -a[1]*p[0] + a[0]*p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1-cos(theta))*(a[0]*a[0]*p[0] + a[0]*a[1]*p[1] + a[0]*a[2]*p[2]);
    temp[1] += (1-cos(theta))*(a[0]*a[1]*p[0] + a[1]*a[1]*p[1] + a[1]*a[2]*p[2]);
    temp[2] += (1-cos(theta))*(a[0]*a[2]*p[0] + a[1]*a[2]*p[1] + a[2]*a[2]*p[2]);

    temp[0] += cos(theta)*p[0];
    temp[1] += cos(theta)*p[1];
    temp[2] += cos(theta)*p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];
}

void Left()
{
    double speed = 3.1415/8;
    rotatePoint(up,speed,eye);
}

void Right()
{
    double speed = 3.1415/8;
    rotatePoint(up,-speed,eye);
}

void Up()
{
    double speed = 3.1415/8;
    double rot_axis[3];
    double look[3];
    look[0] = -eye[0];
    look[1] = -eye[1];
    look[2] = -eye[2];
    crossProduct(look,up,rot_axis);
    normalize(rot_axis);
    rotatePoint(rot_axis,speed,eye);
    rotatePoint(rot_axis,speed,up);
}

void Down()
{
    double speed = 3.1415/8;
    double rot_axis[3];
    double look[3];
    look[0] = -eye[0];
    look[1] = -eye[1];
    look[2] = -eye[2];
    crossProduct(look,up,rot_axis);
    normalize(rot_axis);
    rotatePoint(rot_axis,-speed,eye);
    rotatePoint(rot_axis,-speed,up);
}

void Timer(int value) {
    if (arduino.isConnected()) {
        // Check if data has been read or not
        int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
        if (read_result > 0) {
            if (incomingData == "1") {
                top_finger_1_closing = true;
                top_finger_1_opening = false;
            } else if (incomingData == "2") {
                top_finger_1_closing = false;
                top_finger_1_opening = true;
            } else if (incomingData == "3") {
                top_finger_2_closing = true;
                top_finger_2_opening = false;
            } else if (incomingData == "4") {
                top_finger_2_closing = false;
                top_finger_2_opening = true;
            } else {
                top_finger_1_closing = false;
                top_finger_1_opening = false;
                top_finger_2_closing = false;
                top_finger_2_opening = false;
            }
        }
    }

    if (top_finger_1_closing) {
        top_finger_1_joint_1 = (top_finger_1_joint_1 - 5) % 360;
        if (top_finger_1_joint_1 < -90) {
            top_finger_1_joint_1 = -90;
        }
        top_finger_1_joint_2 = (top_finger_1_joint_2 - 5) % 360;
        if (top_finger_1_joint_2 < -90) {
            top_finger_1_joint_2 = -90;
        }
        top_finger_1_joint_3 = (top_finger_1_joint_3 - 5) % 360;
        if (top_finger_1_joint_3 < -90) {
            top_finger_1_joint_3 = -90;
        }
    } else if (top_finger_1_opening) {
        top_finger_1_joint_1 = (top_finger_1_joint_1 + 5) % 360;
        if (top_finger_1_joint_1 > 0) {
            top_finger_1_joint_1 = 0;
        }
        top_finger_1_joint_2 = (top_finger_1_joint_2 + 5) % 360;
        if (top_finger_1_joint_2 > 0) {
            top_finger_1_joint_2 = 0;
        }
        top_finger_1_joint_3 = (top_finger_1_joint_3 + 5) % 360;
        if (top_finger_1_joint_3 > 0) {
            top_finger_1_joint_3 = 0;
        }
    }

    if (top_finger_2_closing) {
        top_finger_2_joint_1 = (top_finger_2_joint_1 - 5) % 360;
        if (top_finger_2_joint_1 < -90) {
            top_finger_2_joint_1 = -90;
        }
        top_finger_2_joint_2 = (top_finger_2_joint_2 - 5) % 360;
        if (top_finger_2_joint_2 < -90) {
            top_finger_2_joint_2 = -90;
        }
        top_finger_2_joint_3 = (top_finger_2_joint_3 - 5) % 360;
        if (top_finger_2_joint_3 < -90) {
            top_finger_2_joint_3 = -90;
        }
    } else if (top_finger_2_opening) {
        top_finger_2_joint_1 = (top_finger_2_joint_1 + 5) % 360;
        if (top_finger_2_joint_1 > 0) {
            top_finger_2_joint_1 = 0;
        }
        top_finger_2_joint_2 = (top_finger_2_joint_2 + 5) % 360;
        if (top_finger_2_joint_2 > 0) {
            top_finger_2_joint_2 = 0;
        }
        top_finger_2_joint_3 = (top_finger_2_joint_3 + 5) % 360;
        if (top_finger_2_joint_3 > 0) {
            top_finger_2_joint_3 = 0;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(500, Timer, value);
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    glTranslatef(0.0, 0.0, -3.0);
    glTranslatef(0.0, 0.0, -4.0);
    gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],up[0],up[1],up[2]);

    glPushMatrix();

        // Grid - START
        if (showGrid) {
            glBegin(GL_LINES);
            for (int i = -10; i <= 10; ++i) {
                if (i == 0) {
                    glColor3ub(255, 255, 255);
                } else {
                    glColor3ub(0, 255, 0);
                }
                glVertex3f(i, -10, 0);
                glVertex3f(i, 10, 0);
                glVertex3f(-10, i, 0);
                glVertex3f(10, i, 0);
            }
            glEnd();
        }
        // Grid - END

        // First Cube - START
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.5);
            glScalef(2.0, 0.6, 3.5);
            glColor3ub(255, 192, 203);
            glutSolidCube(1.0);
        glPopMatrix();
        // First Cube - END

        // Rotating and Translating the Elbow - START
        glTranslatef(1.0, 0.0, 0.0);
        glRotatef(0.0, 0.0, 0.0, 1.0);
        glTranslatef(2.0, 0.0, 0.0);
        // Rotating and Translating the Elbow - END

        // Second Cube - START
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.5);
            glScalef(4.0, 0.6, 3.5);
            glColor3ub(255, 182, 193);
            glutSolidCube(1.0);
        glPopMatrix();
        // Second Cube - END

        // Top 4 Fingers - START
        glPushMatrix();


            // Finger 1 - START
            glPushMatrix();

                // Rotating the Top Finger 1 (1st joint) - START
                glTranslatef(2.0, 0.3, 0.0);
                glRotatef(top_finger_1_joint_1, 0.0, 0.0, 1.0);
                // Rotating the Top Finger 1 (1st joint) - END

                glRotatef(90.0, 0.0, 0.0, 1.0);
                glTranslatef(0.75, 0.0, 0.0);

                // Finger 1 Part 1 - START
                glPushMatrix();
                    glScalef(1.5, 0.1, 0.1);
                    glColor3ub(255, 105, 180);
                    glutSolidCube(1.0);
                glPopMatrix();
                // Finger 1 Part 1 - END

                // Finger 1 Parts 2 & 3 - START
                glPushMatrix();

                    // Rotating the Top Finger 1 (2nd joint) - START
                    glTranslatef(0.75, 0.0, 0.0);
                    glRotatef(top_finger_1_joint_2, 0.0, 0.0, 1.0);
                    glTranslatef(0.75, 0.0, 0.0);
                    // Rotating the Top Finger 1 (2nd joint) - END

                    // Finger 1 Part 2 - START
                    glPushMatrix();
                        glScalef(1.5, 0.1, 0.1);
                        glColor3ub(255, 105, 180);
                        glutSolidCube(1.0);
                    glPopMatrix();
                    // Finger 1 Part 2 - END

                    // Finger 1 Part 3 - START
                    glPushMatrix();
                        // Rotating the Top Finger 1 (3rd joint) - START
                        glTranslatef(0.75, 0.0, 0.0);
                        glRotatef(top_finger_1_joint_3, 0.0, 0.0, 1.0);
                        glTranslatef(0.75, 0.0, 0.0);
                        // Rotating the Top Finger 1 (3rd joint) - END
                        glScalef(1.5, 0.1, 0.1);
                        glColor3ub(255, 105, 180);
                        glutSolidCube(1.0);
                    glPopMatrix();
                    // Finger 1 Part 3 - END

                glPopMatrix();
                // Finger 1 Parts 2 & 3 - END

            glPopMatrix();
            // Finger 1 - END

            // Finger 2 - START
            glPushMatrix();

                // Rotating the Top Finger 2 (1st joint) - START
                glTranslatef(2.0, 0.3, 0.0);
                glRotatef(top_finger_2_joint_1, 0.0, 0.0, 1.0);
                // Rotating the Top Finger 2 (1st joint) - END

                glRotatef(90.0, 0.0, 0.0, 1.0);
                glTranslatef(0.75, 0.0, 0.0);

                // Finger 2 Part 1 - START
                glPushMatrix();
                    glTranslatef(0.0, 0.0, 1.0);
                    glScalef(1.5, 0.1, 0.1);
                    glColor3ub(255, 105, 180);
                    glutSolidCube(1.0);
                glPopMatrix();
                // Finger 2 Part 1 - END

                // Finger 2 Parts 2 & 3 - START
                glPushMatrix();

                    // Rotating the Top Finger 2 (2nd joint) - START
                    glTranslatef(0.75, 0.0, 0.0);
                    glRotatef(top_finger_2_joint_2, 0.0, 0.0, 1.0);
                    glTranslatef(0.75, 0.0, 0.0);
                    // Rotating the Top Finger 2 (2nd joint) - END

                    // Finger 2 Part 2 - START
                    glPushMatrix();
                        glTranslatef(0.0, 0.0, 1.0);
                        glScalef(1.5, 0.1, 0.1);
                        glColor3ub(255, 105, 180);
                        glutSolidCube(1.0);
                    glPopMatrix();
                    // Finger 2 Part 2 - END

                    // Finger 2 Part 3 - START
                    glPushMatrix();
                        // Rotating the Top Finger 2 (3rd joint) - START
                        glTranslatef(0.75, 0.0, 0.0);
                        glRotatef(top_finger_2_joint_3, 0.0, 0.0, 1.0);
                        glTranslatef(0.75, 0.0, 0.0);
                        // Rotating the Top Finger 2 (3rd joint) - END
                        glTranslatef(0.0, 0.0, 1.0);
                        glScalef(1.5, 0.1, 0.1);
                        glColor3ub(255, 105, 180);
                        glutSolidCube(1.0);
                    glPopMatrix();
                    // Finger 2 Part 3 - END

                glPopMatrix();
                // Finger 2 Parts 2 & 3 - END

            glPopMatrix();
            // Finger 2 - END

            // Finger 3 - START
            glPushMatrix();

                glTranslatef(2.0, 0.3, 0.0);
                glRotatef(0.0, 0.0, 0.0, 1.0);

                glRotatef(90.0, 0.0, 0.0, 1.0);
                glTranslatef(0.75, 0.0, 0.0);

                // Finger 3 Part 1 - START
                glPushMatrix();
                    glTranslatef(0.0, 0.0, 2.0);
                    glScalef(1.5, 0.1, 0.1);
                    glColor3ub(255, 105, 180);
                    glutSolidCube(1.0);
                glPopMatrix();
                // Finger 3 Part 1 - END

                glTranslatef(0.75, 0.0, 0.0);
                glTranslatef(0.75, 0.0, 0.0);

                // Finger 3 Part 2 - START
                glPushMatrix();
                    glTranslatef(0.0, 0.0, 2.0);
                    glScalef(1.5, 0.1, 0.1);
                    glColor3ub(255, 105, 180);
                    glutSolidCube(1.0);
                glPopMatrix();
                // Finger 3 Part 2 - END

                // Finger 3 Part 3 - START
                glPushMatrix();
                    glTranslatef(0.75, 0.0, 0.0);
                    glTranslatef(0.75, 0.0, 0.0);
                    glTranslatef(0.0, 0.0, 2.0);
                    glScalef(1.5, 0.1, 0.1);
                    glColor3ub(255, 105, 180);
                    glutSolidCube(1.0);
                glPopMatrix();
                // Finger 3 Part 3 - END

            glPopMatrix();
            // Finger 3 - END

            // Finger 4 - START
            glPushMatrix();

                glTranslatef(2.0, 0.3, 0.0);
                glRotatef(0.0, 0.0, 0.0, 1.0);

                glRotatef(90.0, 0.0, 0.0, 1.0);
                glTranslatef(0.75, 0.0, 0.0);

                // Finger 4 Part 1 - START
                glPushMatrix();
                    glTranslatef(0.0, 0.0, 3.0);
                    glScalef(1.5, 0.1, 0.1);
                    glColor3ub(255, 105, 180);
                    glutSolidCube(1.0);
                glPopMatrix();
                // Finger 4 Part 1 - END

                glTranslatef(0.75, 0.0, 0.0);
                glTranslatef(0.75, 0.0, 0.0);

                // Finger 4 Part 2 - START
                glPushMatrix();
                    glTranslatef(0.0, 0.0, 3.0);
                    glScalef(1.5, 0.1, 0.1);
                    glColor3ub(255, 105, 180);
                    glutSolidCube(1.0);
                glPopMatrix();
                // Finger 4 Part 2 - END

                // Finger 4 Part 3 - START
                glPushMatrix();
                    glTranslatef(0.75, 0.0, 0.0);
                    glTranslatef(0.75, 0.0, 0.0);
                    glTranslatef(0.0, 0.0, 3.0);
                    glScalef(1.5, 0.1, 0.1);
                    glColor3ub(255, 105, 180);
                    glutSolidCube(1.0);
                glPopMatrix();
                // Finger 4 Part 3 - END

            glPopMatrix();
            // Finger 3 - END

        glPopMatrix();
        // Top 4 Fingers - END

        // Bottom Finger - START
        glPushMatrix();

            glTranslatef(2.0, 0.3, 0.0);
            glRotatef(0.0, 0.0, 0.0, 1.0);

            glRotatef(-45.0, 0.0, 0.0, 1.0);
            glTranslatef(0.75, -0.6, 0.0);

            // Finger 5 Part 1 - START
            glPushMatrix();
                glScalef(1.5, 0.1, 0.1);
                glColor3ub(255, 105, 180);
                glutSolidCube(1.0);
            glPopMatrix();
            // Finger 5 Part 1 - END

            glTranslatef(0.75, 0.0, 0.0);
            glTranslatef(0.75, 0.0, 0.0);

            // Finger 5 Part 2 - START
            glPushMatrix();
                glScalef(1.5, 0.1, 0.1);
                glColor3ub(255, 105, 180);
                glutSolidCube(1.0);
            glPopMatrix();
            // Finger 5 Part 2 - END

            // Finger 5 Part 3 - START
            glPushMatrix();
                glTranslatef(0.75, 0.0, 0.0);
                glTranslatef(0.75, 0.0, 0.0);
                glScalef(1.5, 0.1, 0.1);
                glColor3ub(255, 105, 180);
                glutSolidCube(1.0);
            glPopMatrix();
            // Finger 5 Part 3 - END

        glPopMatrix();
        // Bottom Finger - END

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case '1':
            top_finger_1_closing = true;
            top_finger_1_opening = false;
            break;
        case '2':
            top_finger_1_closing = false;
            top_finger_1_opening = true;
            break;
        case '3':
            top_finger_2_closing = true;
            top_finger_2_opening = false;
            break;
        case '4':
            top_finger_2_closing = false;
            top_finger_2_opening = true;
            break;
        case 'g':
            showGrid = ! showGrid;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

void specialKeys(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_LEFT:
            Left();
            break;
        case GLUT_KEY_RIGHT:
            Right();
            break;
        case GLUT_KEY_UP:
            Up();
            break;
        case GLUT_KEY_DOWN:
            Down();
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
