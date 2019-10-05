#include "graphics.h"
#include "plasma.h" 
#include "distort.h" 
#include "fire.h" 

Plasma plasma;
Distort distort;
Fire fire;

void init(GLFWwindow* window)
{
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

    plasma.init(fb_width, fb_height);

    distort.init(fb_width, fb_height);

    fire.init(fb_width, fb_height);
}

void display()
{
    float current_time = glfwGetTime();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

    //fire.display();
    distort.display();

    //if (current_time < 5) 
        //plasma.display();
    //else
        //distort.display();
}

void destroy()
{
    plasma.destroy();
    distort.destroy();
    fire.destroy();
}
