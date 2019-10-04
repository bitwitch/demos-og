#include "graphics.h" 

typedef unsigned char u8;

#define PI 3.14159265

int fb_width, fb_height;

GLuint vao, vbo; // needed to generate anything???

GLuint shader_program;

GLuint uniform_elapsed_time;

GLuint attribute_v_coord;

GLuint texture_palette, buffer_palette, uniform_palette;

GLubyte *data_palette;

// vertices for full window
const GLfloat vbo_vertices[] = {
    -1, -1, 0, 1,
     1, -1, 0, 1,
    -1,  1, 0, 1,
     1,  1, 0, 1
};

void precalculate(GLubyte *buffer, int select)
{
    int w = fb_width, h = fb_height;
    if (select == 0) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                //buffer[y*w+x] = (GLubyte)(64 + 63 * sin(hypot(0.5*w - x, 0.5*h - y) * 0.0666));
                buffer[y*w+x] = 255;
            }
        }
        
    }
    else
    {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                buffer[y*w+x] = (GLubyte)(64 + 63 * sin((float)x/(37+15*cos((float)y/74)))
                                                  * cos((float)y/(31+11*sin((float)x/57))));
            }
        }
    }
}

GLuint create_plasma_texture(GLubyte* plasma, int select)
{
    precalculate(plasma, select);
    int bufLen = 2*fb_width*2*fb_height;
    GLuint tex_plasma;
    glGenTextures(1, &tex_plasma);
    glBindTexture(GL_TEXTURE_1D, tex_plasma);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, bufLen, 0,
        GL_RED, GL_UNSIGNED_BYTE, plasma);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
    glBindTexture(GL_TEXTURE_1D, 0);
    return tex_plasma;
}

void init(GLFWwindow* window)
{
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

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
    glBindBuffer(GL_TEXTURE_BUFFER, 0);
   
    // create buffer texture
    glGenTextures(1, &texture_palette);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, texture_palette);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R8, buffer_palette);

    // vertex array object
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void display()
{
    float current_time = glfwGetTime();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

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

void destroy()
{
    delete [] data_palette;
}
