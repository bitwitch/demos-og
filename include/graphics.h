#include <algorithm>
#include <vector>
#include <string>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename);
GLuint CreateProgram(const std::vector<GLuint> &shaderList);

void init(GLFWwindow* window);
void display();
void destroy();

