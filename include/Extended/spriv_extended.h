#include "Global.h"
//Load pre-compile shader
GLuint createShaderFromSPIRV(GLenum shaderType, const std::string& spvPath);
//Connect shader program
GLuint createProgram(const std::string& vertexPath, const std::string& fragmentPath);