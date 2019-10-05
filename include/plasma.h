#ifndef PLASMA_H
#define PLASMA_H

#include "graphics.h"

typedef unsigned char u8;

#define PI 3.14159265

struct Plasma
{
    int fb_width, fb_height;
    static const GLfloat vbo_vertices[16];

    GLuint vao, vbo;
    GLuint attribute_v_coord;
    GLuint shader_program;
    GLuint uniform_elapsed_time;
    GLuint texture_palette, buffer_palette, uniform_palette;
    GLubyte *data_palette;

    void init(int width, int height);
    void display();
    void destroy();
};


#endif
