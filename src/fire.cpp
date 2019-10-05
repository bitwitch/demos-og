#include "fire.h"
#include "stb_image.h"

const GLfloat Fire::vbo_vertices[24] = {
    // position     // texCoords
    -1, -1, 0, 1,   0, 1,
     1, -1, 0, 1,   1, 1,
    -1,  1, 0, 1,   0, 0,
     1,  1, 0, 1,   1, 0
};

void Fire::shade_palette(int start, int end, 
    int r1, int g1, int b1, int r2, int g2, int b2)
{
    int i;
    float k;
    for (i=0; i<=end-start; i++)
    {
        k = (float)i/(float)(end-start);
        int r_index = (start+i) * 3;
        data_palette[r_index]   = (u8)(r1+(r2-r1)*k);
        data_palette[r_index+1] = (u8)(g1+(g2-g1)*k);
        data_palette[r_index+2] = (u8)(b1+(b2-b1)*k);
    }
}

// add some random hot spots at the bottom of the buffer

void Fire::heat(GLubyte *dst)
{
    int i, ind, j = (rand() % 1024);
    for (i=0; i<j; i++)
    {
        ind = (rand() % hotspot_width) * 4;
        dst[hotspot_start+ind]   = 255;
        dst[hotspot_start+ind+1] = 255;
        dst[hotspot_start+ind+2] = 255;
    }
}

void Fire::init(int width, int height)
{
    fb_width = width; fb_height = height;

    hotspot_width = fb_width*30;
    hotspot_start = 4*fb_width*fb_height - hotspot_width*4; 

    // Initialize gpu program
	std::vector<GLuint> shaderList;
	shaderList.push_back(LoadShader(GL_VERTEX_SHADER, "fire.vert"));
	shaderList.push_back(LoadShader(GL_FRAGMENT_SHADER, "fire.frag"));
	shader_program = CreateProgram(shaderList);
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	glUseProgram(shader_program);

    // initialize vertex buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_vertices), vbo_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vertex attributes
    attribute_v_coord = glGetAttribLocation(shader_program, "v_coord");
    attribute_tex_coord = glGetAttribLocation(shader_program, "tex_coord");

    // uniforms
    uniform_elapsed_time = glGetUniformLocation(shader_program, "elapsed_time");

    unit_image = 2;
    uniform_image = glGetUniformLocation(shader_program, "image");
    glUniform1i(uniform_image, unit_image);

    unit_palette = 3;
    uniform_palette = glGetUniformLocation(shader_program, "palette");
    glUniform1i(uniform_palette, unit_palette);

    unit_last_frame = 4;
    uniform_last_frame = glGetUniformLocation(shader_program, "last_frame");
    glUniform1i(uniform_last_frame, unit_last_frame);

    GLuint uniform_buffer_width = glGetUniformLocation(shader_program, "buffer_width");
    glUniform1i(uniform_buffer_width, fb_width); 
    GLuint uniform_buffer_height = glGetUniformLocation(shader_program, "buffer_height");
    glUniform1i(uniform_buffer_height, fb_height); 

    // create array to store previous frame pixel data
    data_last_frame = new GLubyte[fb_width*fb_height*4]; // 4 -> RGBA
    memset(data_last_frame, 0, fb_width*fb_height*4);

    // create texture to store previous frame 
    glActiveTexture(GL_TEXTURE0 + unit_last_frame);
    glGenTextures(1, &texture_last_frame);
    glBindTexture(GL_TEXTURE_2D, texture_last_frame);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fb_width, fb_height, 
            0, GL_RGBA, GL_UNSIGNED_BYTE, data_last_frame);
    glBindTexture(GL_TEXTURE_2D, 0);

    // generate palette data
    int buf_size = 256*3; 
    data_palette = new GLubyte[buf_size];

    shade_palette(0, 23, 0, 0, 0, 0, 0, 31);
    shade_palette(24, 47, 0, 0, 31, 63, 0, 0);
    shade_palette(48, 63, 63, 0, 0, 63, 63, 0);
    shade_palette(64, 127, 63, 63, 0, 63, 63, 63);
    shade_palette(128, 255, 63, 63, 63, 63, 63, 63);

    // create palette buffer object
    glGenBuffers(1, &buffer_palette);
    glBindBuffer(GL_TEXTURE_BUFFER, buffer_palette);
    glBufferData(GL_TEXTURE_BUFFER, buf_size, data_palette, GL_STATIC_DRAW);
   
    // create palette buffer texture
    glGenTextures(1, &texture_palette);
    glActiveTexture(GL_TEXTURE0 + unit_palette);
    glBindTexture(GL_TEXTURE_BUFFER, texture_palette);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R8, buffer_palette);

    // load image
    int w, h, n;
    data_image = stbi_load("assets/spongebob_house.png", &w, &h, &n, 4); // rgba
    if (data_image == NULL)
    {
        printf("Error loading image in distort.cpp");
    }

    // create image texture
    glGenTextures(1, &texture_image);
    glActiveTexture(GL_TEXTURE0 + unit_image);
    glBindTexture(GL_TEXTURE_2D, texture_image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
              GL_UNSIGNED_BYTE, data_image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data_image);

    // vertex array object
    glGenVertexArrays(1, &vao);
}

void Fire::display()
{
    float current_time = glfwGetTime();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);

	glUseProgram(shader_program);

    glUniform1f(uniform_elapsed_time, current_time); 

    // last_frame texture
    glActiveTexture(GL_TEXTURE0 + unit_last_frame);
    glBindTexture(GL_TEXTURE_2D, texture_last_frame);

    heat(data_last_frame);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fb_width, fb_height, 
            0, GL_RGBA, GL_UNSIGNED_BYTE, data_last_frame);

    // color palette
    glActiveTexture(GL_TEXTURE0 + unit_palette);
    glBindTexture(GL_TEXTURE_BUFFER, texture_palette);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R8, buffer_palette);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(attribute_v_coord);
    glEnableVertexAttribArray(attribute_tex_coord);

    glVertexAttribPointer(
        attribute_v_coord,  // attribute
        4,                  // number of elements per vertex, here (x,y,z,w)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        6*sizeof(float),    // stride 
        0                   // offset of first element
    );

    glVertexAttribPointer(
        attribute_tex_coord,     // attribute
        2,                       // number of elements per vertex, here (texX,texY)
        GL_FLOAT,                // the type of each element
        GL_FALSE,                // take our values as-is
        6*sizeof(float),         // stride 
        (void*)(4*sizeof(float)) // offset of first element
    );

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // oof, expensive
    //glReadPixels(0, 0, fb_width, fb_height, 
        //GL_RGBA, GL_UNSIGNED_BYTE, data_last_frame);

    // NOTE: glfwSwapBuffers() called by main loop!!!

}

void Fire::destroy()
{
    delete [] data_palette; 
    delete [] data_last_frame;

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
}


