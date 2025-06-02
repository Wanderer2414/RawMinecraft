#include "Global.h"
#include "spriv_extended.h"

std::vector<char> loadSPIRV(const std::string& src) {
    std::ifstream file(src, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open SPIR-V binary");
        exit(0);
    }
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Failed to read SPIR-V binary");
    }
    file.close();
    return buffer;
}

GLuint createShaderFromSPIRV(GLenum shaderType, const std::string& src) {
    std::vector<char> spirv = loadSPIRV(src);

    GLuint shader = glCreateShader(shaderType);
    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), (GLsizei)spirv.size());
    glSpecializeShader(shader, "main", 0, nullptr, nullptr); // "main" entry point

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "SPIR-V Shader compilation error: " << log << std::endl;
        return 0;
    }
    return shader;
}

GLuint createProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    GLuint vertexShader = createShaderFromSPIRV(GL_VERTEX_SHADER, vertexPath);
    GLuint fragmentShader = createShaderFromSPIRV(GL_FRAGMENT_SHADER, fragmentPath);
    if (vertexShader == 0 || fragmentShader == 0) {
        std::cerr << "Failed to create shaders." << std::endl;
        exit(0);
    }
    else {
        std::cout << "Shader created: " << std::endl;
        std::cout << "Vertex shader: " << vertexPath << std::endl;
        std::cout << "Fragment shader: " << fragmentPath << std::endl;
    }
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    return program;
}