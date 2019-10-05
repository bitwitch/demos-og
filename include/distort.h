#ifndef DISTORT_H
#define DISTORT_H

#include "graphics.h"

typedef unsigned char u8;

// vertices for full window
struct Distort
{
    int fb_width, fb_height;
    static const GLfloat vbo_vertices[24];

    GLuint vao, vbo;
    GLuint attribute_v_coord, attribute_tex_coord;
    GLuint shader_program;
    GLuint uniform_elapsed_time;

    GLuint texture_image, uniform_image, unit_image;
    GLubyte *data_image;

    void init(int width, int height);
    void display();
    void destroy();
};


#endif
