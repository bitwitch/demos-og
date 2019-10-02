#include "graphics.h" 

typedef unsigned char u8;

int fb_width, fb_height;

GLuint theProgram;
GLuint uniElapsedTime;
GLuint vbo;
GLuint vao;

GLubyte *plasma1;
GLuint tex_plasma1;
const int plasma1_tex_unit = 0;
GLuint plasma1_sampler;

// full window, 2 triangles (one quad)
const float vertexData[] = {
    -1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,

    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f
};

void precalculate(GLubyte *buffer, int select)
{
    memset(buffer, 255, 2*fb_width*2*fb_height);
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

void init(GLFWwindow *window)
{
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

	// Initialize gpu program
	std::vector<GLuint> shaderList;
	shaderList.push_back(LoadShader(GL_VERTEX_SHADER, "shader.vert"));
	shaderList.push_back(LoadShader(GL_FRAGMENT_SHADER, "shader.frag"));
	theProgram = CreateProgram(shaderList);
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	// Initialize Vertex Buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(theProgram);

    // Initialize uniforms
    GLuint uniLoopDuration = glGetUniformLocation(theProgram, "loopDuration");
    glUniform1f(uniLoopDuration, 5.0f);
    uniElapsedTime = glGetUniformLocation(theProgram, "elapsedTime");

    // Initialize texture
    int buf_size = 2*fb_width*2*fb_height;
    printf("buf_size: %d\n", buf_size);
    printf("max size: %d\n", GL_MAX_TEXTURE_SIZE);

    plasma1 = new GLubyte[buf_size];    
    memset(plasma1, 255, buf_size);
    printf("middle check: %d\n", plasma1[50]);

    GLuint uniPlasma1 = glGetUniformLocation(theProgram, "plasma1");
	glUniform1i(uniPlasma1, tex_plasma1);

    //tex_plasma1 = create_plasma_texture(plasma1, 0);

	glGenTextures(1, &tex_plasma1);
	glBindTexture(GL_TEXTURE_1D, tex_plasma1);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, buf_size, 0,
		GL_RED, GL_UNSIGNED_BYTE, plasma1);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);

    glActiveTexture(GL_TEXTURE0 + tex_plasma1);
    glBindTexture(GL_TEXTURE_1D, tex_plasma1); 

    glGenSamplers(1, &plasma1_sampler);
	glSamplerParameteri(plasma1_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(plasma1_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(plasma1_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glBindSampler(tex_plasma1, plasma1_sampler);

    // bind vertex array object to context
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    // set culling options
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void display()
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(theProgram);

    // pass uniform data
    glUniform1f(uniElapsedTime, glfwGetTime()); 

    // pass vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6); 
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
}

void destroy() {
    delete [] plasma1;
}
