#include <math.h>
#include <iostream>
#include <ctime>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "visuals.h"
#include <sys/time.h>
#include "shader.hpp"

#define PI 3.14159265359

#define NANO 0.000000001

// This will be used with shader
//GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint programID_1, programID_2;

timespec last_time, current_time;
double seconds, nseconds, dt;

double xpos = 0;
int dir = 1;

GLuint MatrixID; // Handler Matrix for moving the cam
glm::mat4 MVP; // FInal Homogeneous Matrix
glm::mat4 Projection,View,Model;
void Idle()
{

    clock_gettime(CLOCK_MONOTONIC, &current_time); // update current timer

    if(rot_angle == -1) // only happens the first time Idel is called . This avoid timer problems.
    {
        rot_angle = 0;
        Model = glm::mat4(1.0f);
        Model = glm::scale(Model, glm::vec3(0.5f));
    }
    else
    {
        seconds  = current_time.tv_sec  - last_time.tv_sec;
        nseconds = current_time.tv_nsec - last_time.tv_nsec;
        dt       = nseconds * NANO;
    
        /* Compute animation here*/
        if (g_eCurrentScene == 1)
        {
            rot_angle = 60.0;

            if (dt > 0.0) {
                Model = glm::rotate(Model, float(dt * rot_angle), glm::vec3(1,0,0));
            }
        }

        /* Compute animation here*/
        if (g_eCurrentScene == 2)
        {
            // rotation speed is 60 degrees per second = .06 per ms;

            rot_angle = 60.0;
            
            // std::cout << "ang: " << float(rot_angle)  << std::endl;

            if (dt > 0.0) {
                Model = glm::rotate(Model, float(dt * rot_angle), glm::vec3(1,0,0));
                Model = glm::rotate(Model, float(dt * rot_angle) * 0.8f, glm::vec3(0,1,0));
            }
        }

        /* Compute animation here*/
        if (g_eCurrentScene == 3)
        {
            // rotation speed is 60 degrees per second = .06 per ms;

            rot_angle = 60.0;
            
            // std::cout << "ang: " << float(rot_angle)  << std::endl;

            if (dt > 0.0) {
                Model = glm::rotate(Model, float(dt * rot_angle), glm::vec3(1,0,0));
                Model = glm::rotate(Model, float(dt * rot_angle) * 0.8f, glm::vec3(0,1,0));
                Model = glm::rotate(Model, float(dt * rot_angle) * 0.1f, glm::vec3(0,0,1));
            }
        }


        if (g_eCurrentScene == 4)
        {
            if (xpos > 4.0 || xpos < -4.0)
                dir *= -1;

            if (dt > 0.0) {
                float dx = dt * dir;
                xpos += dx;
                // std::cout << "x: " << float(xpos) << std::endl;
                Model = glm::translate(Model, glm::vec3(dx, 0.0f, 0.0f)); // where x, y, z is axis of rotation (e.g. 0 1 0)
            }
        }

        if (g_eCurrentScene == 5)
        {
            rot_angle = 60.0;

            if (xpos > 4.0 || xpos < -4.0)
                dir *= -1;

            if (dt > 0.0) {
                float dx = dt * dir;
                xpos += dx;
                // std::cout << "x: " << float(xpos) << std::endl;
                Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, dx)); // where x, y, z is axis of rotation (e.g. 0 1 0)

                Model = glm::rotate(Model, float(dt * rot_angle), glm::vec3(1,0,0));
                Model = glm::rotate(Model, float(dt * rot_angle) * 0.8f, glm::vec3(0,1,0));
                Model = glm::rotate(Model, float(dt * rot_angle) * 0.1f, glm::vec3(0,0,1));
            }
        }

    }
    glutPostRedisplay();

    clock_gettime(CLOCK_MONOTONIC, &last_time); // update when the last timer;


}

void ReshapeGL( int w, int h )
{
    std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

    if ( h == 0)                                        // Prevent a divide-by-zero error
    {
        h = 1;                                      // Making Height Equal One
    }

    g_iWindowWidth = w;
    g_iWindowHeight = h;

    glViewport( 0, 0, 800, 600 );

}

void ResetModel()
{
    Model = glm::mat4(1.0f);
}

void KeyboardGL( unsigned char c, int x, int y )
{
    // Store the current scene so we can test if it has changed later.
    unsigned char currentScene = g_eCurrentScene;

    switch ( c )
    {
    case '1':
    {
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );                         // White background
        g_eCurrentScene = 1;
        ResetModel();
    }
        break;
    case '2':
    {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );                         // Black background
        g_eCurrentScene = 2;
        ResetModel();
    }
        break;
    case '3':
    {
        glClearColor( 0.27f, 0.27f, 0.27f, 1.0f );                      // Dark-Gray background
        g_eCurrentScene = 3;
        ResetModel();
    }
        break;
    case '4':
    {
        glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );                      // Light-Gray background
        g_eCurrentScene = 4;
        ResetModel();
    }
        break;
    case '5':
    {
        glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );                      // Light-Gray background
        g_eCurrentScene = 5;
        ResetModel();
    }
        break;
    case 's':
    case 'S':
    {
        std::cout << "Shade Model: GL_SMOOTH" << std::endl;
        // Switch to smooth shading model
        glShadeModel( GL_SMOOTH );
    }
        break;
    case 'f':
    case 'F':
    {
        std::cout << "Shade Model: GL_FLAT" << std::endl;
        // Switch to flat shading model
        glShadeModel( GL_FLAT );
    }
        break;
    case '\033': // escape quits
    case '\015': // Enter quits
    case 'Q':    // Q quits
    case 'q':    // q (or escape) quits
    {
        // Cleanup up and quit
        Cleanup(0);
    }
        break;

    case '\72': //arrow up
    {

    }
        break;


    }


    if ( currentScene != g_eCurrentScene )
    {
        std::cout << "Changed Render Scene: " << int( g_eCurrentScene ) << std::endl;
    }

    glutPostRedisplay();
}


void Cleanup( int errorCode, bool bExit )
{
    if ( g_iGLUTWindowHandle != 0 )
    {
        glutDestroyWindow( g_iGLUTWindowHandle );
        g_iGLUTWindowHandle = 0;
    }

    if ( bExit )
    {
        exit( errorCode );
    }
}


void DisplayGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
    // and the depth buffer

    switch ( g_eCurrentScene )
    {
    case 1:
    {
        RenderScene1();
    }
        break;
    case 2:
    {
        RenderScene2();
    }
        break;
    case 3:
    {
        RenderScene3();
    }
        break;
    case 4:
    {
        RenderScene4();
    }
        break;
    case 5:
    {
        RenderScene5();
    }
        break;
    }


    glutSwapBuffers();
    // All drawing commands applied to the
    // hidden buffer, so now, bring forward
    // the hidden buffer and hide the visible one

}

void RenderScene1()
{

    // Use our shader
    glUseProgram(programID_1);

    glm::mat4 MVP = Model;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );
    
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // From index 0 to 3 -> 1 triangle

    glDisableVertexAttribArray(0);


}

void RenderScene2()
{
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    View       = glm::lookAt(
                glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                );
    // Model matrix : an identity matrix (model will be at the origin)
    
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around


    // Use our shader
    glUseProgram(programID_2);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3 * 12); // From index 0 to 3 -> 1 triangle

    glDisableVertexAttribArray(0);

}

void RenderScene3()
{

    Projection = glm::ortho( -10.0f,  10.0f,   10.0f,   -10.0f,   0.0f,   10.0f     );


    // Camera matrix
    View       = glm::lookAt(
                glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                );
    // Model matrix : an identity matrix (model will be at the origin)
    //Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around


    // Use our shader
    glUseProgram(programID_2);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3 * 12); // From index 0 to 3 -> 1 triangle

    glDisableVertexAttribArray(0);

}
void RenderScene4()
{

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    View       = glm::lookAt(
                glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                );
    // Model matrix : being updated by idle
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around


    // Use our shader
    glUseProgram(programID_2);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3 * 12); // From index 0 to 3 -> 1 triangle

    glDisableVertexAttribArray(0);

}

void RenderScene5()
{

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    View       = glm::lookAt(
                glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                );
    // Model matrix : being updated by idle
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around


    // Use our shader
    glUseProgram(programID_2);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3 * 12); // From index 0 to 3 -> 1 triangle

    glDisableVertexAttribArray(0);

}




void SetupGL() //
{

    //Parameter handling
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    // polygon rendering mode
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

    //Set up light source
    GLfloat light_position[] = { 0.0, 30.0,-50.0,0.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Black background
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    // Register GLUT callbacks
    glutDisplayFunc(DisplayGL);
    glutKeyboardFunc(KeyboardGL);
    glutReshapeFunc(ReshapeGL);

    // Setup initial GL State
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClearDepth( 1.0f );

    //
    // Init GLEW
    if ( glewInit() != GLEW_OK )
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        exit(-1);
    }

    // Setup initial GL State
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClearDepth( 1.0f );

    std::cout << "Initialise OpenGL: Success!" << std::endl;

    //VAO
    //        glGenVertexArrays(1, &VertexArrayID);
    //        glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID_1 = LoadShaders( "SimpleVertexShader.glsl", "SimpleFragmentShader.glsl" );
    programID_2 = LoadShaders( "SimpleTransform.glsl", "SimpleFragmentShader.glsl" );

    // static const GLfloat g_vertex_buffer_data[] = {
    //     -1.0f, -1.0f, 0.0f,
    //     1.0f, -1.0f, 0.0f,
    //     0.0f,  1.0f, 0.0f,
    // };

    //VBO
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,

        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,

        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,

        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,

        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}
