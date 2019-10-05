#include "distort.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const GLfloat Distort::vbo_vertices[24] = {
    // position     // texCoords
    -1, -1, 0, 1,   0, 0,
     1, -1, 0, 1,   1, 0,
    -1,  1, 0, 1,   1, 1,
     1,  1, 0, 1,   0, 1
};

void Distort::init(int width, int height)
{
    fb_width = width; fb_height = height;

    // Initialize gpu program
	std::vector<GLuint> shaderList;
	shaderList.push_back(LoadShader(GL_VERTEX_SHADER, "distort.vert"));
	shaderList.push_back(LoadShader(GL_FRAGMENT_SHADER, "distort.frag"));
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
    uniform_elapsed_time = glGetUniformLocation(shader_program, "elapsedTime");

    unit_image = 2;
    uniform_image = glGetUniformLocation(shader_program, "image");
    glUniform1i(uniform_image, unit_image);

    GLuint uniform_buffer_width = glGetUniformLocation(shader_program, "buffer_width");
    glUniform1i(uniform_buffer_width, fb_width); 
    GLuint uniform_buffer_height = glGetUniformLocation(shader_program, "buffer_height");
    glUniform1i(uniform_buffer_height, fb_height); 

    // load image
    int w, h, n;
    data_image = stbi_load("assets/xen.png", &w, &h, &n, 4); // rgba
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

void Distort::display()
{
    float current_time = glfwGetTime();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);

	glUseProgram(shader_program);

    glUniform1f(uniform_elapsed_time, current_time); 

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

    // NOTE: glfwSwapBuffers() called by main loop!!!

}

void Distort::destroy()
{
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
}

