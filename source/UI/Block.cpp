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

    glm::vec2 tex_coord[14];
    tex_coord[0] = {1.0/3, 0};
    tex_coord[1] = {2.0/3, 0};
    tex_coord[2] = {2.0/3, 3.0/4};
    tex_coord[3] = {1.0/3, 3.0/4};
    tex_coord[4] = {1.0/3, 1.0/4};
    tex_coord[5] = {2.0/3, 1.0/4};
    tex_coord[6] = {2.0/3, 2.0/4};
    tex_coord[7] = {1.0/3, 2.0/4};
    tex_coord[8] = {1.0/3, 1};
    tex_coord[9] = {2.0/3, 1};
    tex_coord[10] = {0, 1.0/4};
    tex_coord[11] = {1, 1.0/4};
    tex_coord[12] = {1, 2.0/4};
    tex_coord[13] = {0, 2.0/4};

    glGenBuffers(1, &__blockTexture);
    glBindBuffer(GL_ARRAY_BUFFER, __blockTexture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*14*2, &tex_coord[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

MyCraft::Block::Block():__hoverPlane(-1), __position(0, 0, 0) {

}
MyCraft::Block::~Block() {
}

MyCraft::BlockCatogary::Catogary MyCraft::Block::getType() const {
    return __type;
}

char MyCraft::Block::getHoverPlane() const {
    return __hoverPlane;
}
void MyCraft::Block::setPosition(const float& x, const float& y, const float& z) {
    __position = {x, y, z};
}
void MyCraft::Block::setPosition(const glm::vec3& position) {
    __position = position;
}
void MyCraft::Block::setHoverPlane(const char& index) {
    __hoverPlane = index;
}
void MyCraft::Block::setType(const BlockCatogary::Catogary& type) {
    __type = type;
}
void MyCraft::Block::glDraw() const {
    glUseProgram(MyBase3D::ShaderStorage::Default->GetCubeShader());

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::Default->getBlockSet());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, MyCraft::BlockCatogary::Default->getTexCoord());
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::Default->getImageBlockIndices());

    GLuint originPoint;
    
    glGenBuffers(1, &originPoint);
    glBindBuffer(GL_UNIFORM_BUFFER, originPoint);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*3, &__position[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, originPoint);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, MyCraft::BlockCatogary::Default->getBlock(__type));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    if (__hoverPlane!=-1) {
        glUseProgram(MyBase3D::ShaderStorage::Default->GetMarginShader());
        glLineWidth(3);
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::Default->getBlockSet());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindBufferBase(GL_UNIFORM_BUFFER, 1, originPoint);
        float buffer[8] = {
            1, 1, 1, 0,
            0, 0, 0, 1
        };
        GLuint margin;
        glGenBuffers(1, &margin);
        glBindBuffer(GL_UNIFORM_BUFFER, margin);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*8, buffer, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, margin);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::Default->getMarginBlockIndices());
        glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_INT, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &margin);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &originPoint);
}