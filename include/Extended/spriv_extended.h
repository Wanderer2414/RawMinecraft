#include "Global.h"

GLuint createShaderFromSPIRV(GLenum shaderType, const std::string& spvPath);
GLuint createProgram(const string& vertexPath, const string& fragmentPath);