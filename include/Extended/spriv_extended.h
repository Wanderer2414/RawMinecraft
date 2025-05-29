#include "Global.h"

GLuint createShaderFromSPIRV(GLenum shaderType, const std::string& spvPath);
GLuint createProgram(const std::string& vertexPath, const std::string& fragmentPath);