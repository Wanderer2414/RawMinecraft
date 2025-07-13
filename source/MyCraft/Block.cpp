#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"

MyCraft::BlockCatogary::BlockCatogary() {
    __ptr.resize(3, 0);
    __ptr[0] = 0;
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/images/Dirt.png", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
        exit(0);
    }
    glGenTextures(1, &__ptr[1]);
    glBindTexture(GL_TEXTURE_2D, __ptr[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    data = stbi_load("assets/images/Grass.png", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
        exit(0);
    }
    glGenTextures(1, &__ptr[2]);
    glBindTexture(GL_TEXTURE_2D, __ptr[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);

    glm::vec4 tex_coord[14];
    tex_coord[0] = {1.0/3, 0, 0,0};
    tex_coord[1] = {2.0/3, 0, 0,0};
    tex_coord[2] = {2.0/3, 3.0/4, 0,0};
    tex_coord[3] = {1.0/3, 3.0/4, 0,0};
    tex_coord[4] = {1.0/3, 1.0/4, 0,0};
    tex_coord[5] = {2.0/3, 1.0/4, 0,0};
    tex_coord[6] = {2.0/3, 2.0/4, 0,0};
    tex_coord[7] = {1.0/3, 2.0/4, 0,0};
    tex_coord[8] = {1.0/3, 1, 0,0};
    tex_coord[9] = {2.0/3, 1, 0,0};
    tex_coord[10] = {0, 1.0/4, 0,0};
    tex_coord[11] = {1, 1.0/4, 0,0};
    tex_coord[12] = {1, 2.0/4, 0,0};
    tex_coord[13] = {0, 2.0/4, 0,0};

    glGenBuffers(1, &__blockTexture);
    glBindBuffer(GL_UNIFORM_BUFFER, __blockTexture);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*14*4, &tex_coord[0], GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
MyCraft::BlockCatogary::~BlockCatogary() {
    glDeleteTextures(1, &__ptr[1]);
    glDeleteTextures(1, &__ptr[2]);
    glDeleteBuffers(1, &__blockTexture);
}

GLuint MyCraft::BlockCatogary::getBlock(const int& index) const {
    return __ptr[index];
}

GLuint MyCraft::BlockCatogary::getTexCoord() const {
    return __blockTexture;
}

// void MyCraft::Block::glDraw() const {
//     if (__hoverPlane!=-1) {
//     }
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &originPoint);
// }