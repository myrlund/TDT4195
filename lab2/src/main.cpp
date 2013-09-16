#include <math.h>
#include <iostream>
#include <ctime>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "visuals.h"

#pragma comment(lib, "glew32.lib")
#define GLEW_STATIC
//--------- Global Variables
int g_iWindowWidth = 512;
int g_iWindowHeight = 512;
int g_iGLUTWindowHandle = 0;
int g_iErrorCode = 0;


unsigned char g_eCurrentScene = 1;

//---------- Main program
int main(int argc, char **argv) {

    glutInit(&argc, argv);

    // Setting up the Display
    // RGB color model + Alpha Channel = GLUT_RGBA
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

    // Configure Window Position
    int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitDisplayMode( GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH );

    int iWindowWidth = 512;
    int iWindowHeight = 512;
    
    glutInitWindowPosition( iScreenWidth / 2 - iWindowWidth / 2, iScreenHeight / 2 - iWindowHeight / 2 );
    glutInitWindowSize( iWindowWidth, iWindowHeight );

    g_iGLUTWindowHandle = glutCreateWindow( "OpenGL" );

    SetupGL();

    // Call to the drawing function
    glutDisplayFunc(DisplayGL);
    glutReshapeFunc(ReshapeGL);
    
    // Loop require by OpenGL
    glutMainLoop();
    return 0;
}
//-----------------------------------------------------------


