//
//  application.m
//  Game
//
//  Created by YueYouqian on 16/6/23.
//
//


#include "application.h"

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>


int Application::run()
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        usleep(static_cast<useconds_t>(1000));
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
        
        
        
    }
    
    glfwTerminate();
    
    
    return 0;
}


