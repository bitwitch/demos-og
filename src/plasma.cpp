#include "plasma.h"

const GLfloat Plasma::vbo_vertices[16] = {
    -1, -1, 0, 1,
     1, -1, 0, 1,
    -1,  1, 0, 1,
     1,  1, 0, 1
};

void Plasma::init(int width, int height)
{
    fb_width = width; fb_height = height;

    // Initialize gpu program
	std::vector<GLuint> shaderList;
	shaderList.push_back(LoadShader(GL_VERTEX_SHADER, "shader.vert"));
	shaderList.push_back(LoadShader(GL_FRAGMENT_SHADER, "shader.frag"));
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

    // uniforms
    uniform_elapsed_time = glGetUniformLocation(shader_program, "elapsedTime");
    uniform_palette = glGetUniformLocation(shader_program, "palette");

    GLuint uniform_buffer_width = glGetUniformLocation(shader_program, "buffer_width");
    glUniform1i(uniform_buffer_width, fb_width); 
    GLuint uniform_buffer_height = glGetUniformLocation(shader_program, "buffer_height");
    glUniform1i(uniform_buffer_height, fb_height); 

    // generate palette table data
    int buf_size = 256*3; 
    data_palette = new GLubyte[buf_size];
    memset(data_palette, 0, buf_size);

    // create palette table buffer object
    glGenBuffers(1, &buffer_palette);
    glBindBuffer(GL_TEXTURE_BUFFER, buffer_palette);
    glBufferData(GL_TEXTURE_BUFFER, buf_size, data_palette, GL_DYNAMIC_DRAW);
   
    // create buffer texture
    glGenTextures(1, &texture_palette);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, texture_palette);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R8, buffer_palette);

    // vertex array object
    glGenVertexArrays(1, &vao);
}

void Plasma::display()
{
    float current_time = glfwGetTime();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);

	glUseProgram(shader_program);

    glUniform1f(uniform_elapsed_time, current_time); 

    GLubyte r, g, b;
    int i, dst = 0;
    for (i=0; i<256; ++i) 
    {
        r = (GLubyte)(32 + 31 * cos(i * PI / 128 + current_time/7));
        g = (GLubyte)(32 + 31 * sin(i * PI / 128 + current_time/6));
        b = (GLubyte)(32 - 31 * cos(i * PI / 128 + current_time/8));

        data_palette[dst++] = r; 
        data_palette[dst++] = g; 
        data_palette[dst++] = b; 
    }

    glBindBuffer(GL_TEXTURE_BUFFER, buffer_palette);
    glBufferData(GL_TEXTURE_BUFFER, 256*3, data_palette, GL_DYNAMIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, texture_palette);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R8, buffer_palette);
    glUniform1i(uniform_palette, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(attribute_v_coord);

    glVertexAttribPointer(
        attribute_v_coord,  // attribute
        4,                  // number of elements per vertex, here (x,y,z,w)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // NOTE: glfwSwapBuffers() called by main loop!!!

}

void Plasma::destroy()
{
    delete [] data_palette;

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &buffer_palette);

    glDeleteTextures(1, &texture_palette);

    glDeleteVertexArrays(1, &vao);
}

