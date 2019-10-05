#ifndef FIRE_H
#define FIRE_H

#include "graphics.h"

typedef unsigned char u8;

// vertices for full window
struct Fire
{
    int fb_width, fb_height;
    int hotspot_start, hotspot_width;
    static const GLfloat vbo_vertices[24];

    GLuint vao, vbo;
    GLuint attribute_v_coord, attribute_tex_coord;
    GLuint shader_program;
    GLuint uniform_elapsed_time;

    GLuint texture_image, uniform_image, unit_image;
    GLubyte *data_image;

    GLuint texture_palette, buffer_palette, uniform_palette, unit_palette;
    GLubyte *data_palette;

    GLuint texture_last_frame, uniform_last_frame, unit_last_frame;
    GLubyte *data_last_frame;

    void init(int width, int height);
    void display();
    void destroy();

    void shade_palette(int start, int end, 
        int r1, int g1, int b1, int r2, int g2, int b2);
    void heat(GLubyte *dst);
};


#endif

