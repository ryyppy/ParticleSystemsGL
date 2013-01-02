//
//  LUtil.cpp
//  ParticleSystem
//
//  Created by Patrick Stapfer on 26.12.12.
//  Copyright (c) 2012 Patrick Stapfer. All rights reserved.
//

#include "LOpenGL.h"
#include "Window.h"

Game* Window::m_game;
Camera* Window::m_camera;

Window::Window(int argc, char* args[], Game* game, Camera* camera): m_window(0){
    Window::m_game = game;
    Window::m_camera = camera;
    
    //Initialize FreeGLUT
    glutInit( &argc, args );
    
    //Create OpenGL 2.1 context
    glutInitContextVersion( 2, 1 );
    
    //Create Double Buffered Window
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
    this->m_window = glutCreateWindow( "Simple Particle Systems" );
    
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
    //Enable Texturizing
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glFrontFace(GL_CW);
    
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    
    //Check for error
    GLenum error = glGetError();
    
    if( error != GL_NO_ERROR )
    {
        throw error;
    }
    
    //Set rendering function
    glutDisplayFunc( Window::render );
    
    //Set reshape callback
    glutReshapeFunc (reshape);
    
    //Set the run-callback for the mainloop
    glutTimerFunc( 1000 / Window::SCREEN_FPS, Window::run, 0 );

    
    
    
    
}

bool Window::destroy(){
    if(this->m_window > 0){
        glutDestroyWindow(this->m_window);
        return true;
    }
    return false;
}


void Window::render()
{
    gluPerspective (90, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 100.0);
    
    glFrustum( -SCREEN_WIDTH,
              SCREEN_WIDTH,
              -SCREEN_HEIGHT,
              SCREEN_HEIGHT,
              0.0005f,
              SCREEN_HEIGHT);
    
    //Clear and set the image to render
    glClearDepth (1);
    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Reset Worldmatrix to (0,0,0)
    glLoadIdentity();
    
    //Reposition the camera perspective
    Window::m_camera->draw();
    
    //Call to the game-logic to draw it's drawables
    Window::m_game->draw();
    
    //Update screen
    glutSwapBuffers();
}   

void Window::reshape (int width, int height) {
    glViewport (0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

void Window::run( int val ){    
    //Frame logic
    Window::render();
    
    //Run frame one more time
    glutTimerFunc( 1000 / Window::SCREEN_FPS, Window::run, val );
}
