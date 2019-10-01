#include "graphics.h" 

typedef unsigned char u8;

GLuint theProgram;
GLuint uniElapsedTime;
GLuint vbo;
GLuint vao;

GLubyte *plasma1;
GLubyte *plasma2;

int fb_width, fb_height;

float *vertexData;

void precalculate(GLubyte *buffer, int select)
{
    int w = fb_width, h = fb_height;
    if (select == 0) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                buffer[y*w+x] = (GLubyte)(64 + 63 * sin(hypot(0.5*w - x, 0.5*h - y) * 0.0666));
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
        GL_RED, GL_UNSIGNED_BYTE, &plasma[0]);
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
	theProgram = CreateProgram(shaderList);
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	// Initialize Vertex Buffer
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

    // initialize precalculated plasma textures
    int bufSize = 2*fb_width*2*fb_height;
    plasma1 = new GLubyte[bufSize];
    plasma2 = new GLubyte[bufSize];
    GLuint tex_plasma1 = create_plasma_texture(plasma1, 0);
    GLuint tex_plasma2 = create_plasma_texture(plasma2, 1);

    // Initialize uniforms
    glUseProgram(theProgram);

    GLuint uniLoopDuration = glGetUniformLocation(theProgram, "loopDuration");
    glUniform1f(uniLoopDuration, 5.0f);

    GLuint uniFbWidth = glGetUniformLocation(theProgram, "fbWidth");
    glUniform1i(uniFbWidth, fb_width);
    GLuint uniFbHeight = glGetUniformLocation(theProgram, "fbHeight");
    glUniform1i(uniFbHeight, fb_height);



// TODO
// TODO
// TODO
// TODO
//GLuint uniPlasma1 = glGetUniformLocation(theProgram, "plasma1");
//glUniform1i(uniPlasma1, g_gaussTexUnit);

//glActiveTexture(GL_TEXTURE0 + g_gaussTexUnit);
//glBindTexture(GL_TEXTURE_1D, g_gaussTextures[g_currTexture]);

//glGenSamplers(1, &g_gaussSampler);
//glSamplerParameteri(g_gaussSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//glSamplerParameteri(g_gaussSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//glSamplerParameteri(g_gaussSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

//glBindSampler(g_gaussTexUnit, g_gaussSampler);



    uniElapsedTime = glGetUniformLocation(theProgram, "elapsedTime");

    // bind vertex array object to context
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(theProgram);

    // pass uniform data
    glUniform1f(uniElapsedTime, glfwGetTime()); 

    // pass vertex data
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);

	//glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	glUseProgram(0);
}

void destroy()
{
    delete [] plasma1;
    delete [] plasma2;
    delete [] vertexData;
}