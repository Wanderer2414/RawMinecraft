#include "Block.h"
#include "General.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "glm/fwd.hpp"

BlockCatogary::BlockCatogary() {
    pPtr.resize(2, 0);
    pPtr[0] = 0;
    Image image;
    image.loadFromFile("assets/images/Dirt.png");
    glGenTextures(1, &pPtr[1]);
    glBindTexture(GL_TEXTURE_2D, pPtr[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x,image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glm::vec2 tex_coord[24];
    tex_coord[0] = {1.0/3, 0};
    tex_coord[1] = {2.0/3, 0};
    tex_coord[2] = {2.0/3, 1.0/4};
    tex_coord[3] = {1.0/3, 1.0/4};

    tex_coord[4] = {1.0/3, 1.0/4};
    tex_coord[5] = {2.0/3, 1.0/4};
    tex_coord[6] = {2.0/3, 2.0/4};
    tex_coord[7] = {1.0/3, 2.0/4};

    tex_coord[8] = {1.0/3, 2.0/4};
    tex_coord[9] = {2.0/3, 2.0/4};
    tex_coord[10] = {2.0/3, 3.0/4};
    tex_coord[11] = {1.0/3, 3.0/4};

    tex_coord[12] = {1.0/3, 3.0/4};
    tex_coord[13] = {2.0/3, 3.0/4};
    tex_coord[14] = {2.0/3, 1};
    tex_coord[15] = {1.0/3, 1};

    tex_coord[16] = {2.0/3, 2.0/4};
    tex_coord[17] = {2.0/3, 1.0/4};
    tex_coord[18] = {1, 1.0/4};
    tex_coord[19] = {1, 2.0/4};

    tex_coord[20] = {1.0/3, 1.0/4};
    tex_coord[21] = {1.0/3, 2.0/4};
    tex_coord[22] = {0, 2.0/4};
    tex_coord[23] = {0, 1.0/4};

    glGenBuffers(1, &BlockTexture);
    glBindBuffer(GL_ARRAY_BUFFER, BlockTexture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*24*2, &tex_coord[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
BlockCatogary::~BlockCatogary() {
    glDeleteTextures(1, &pPtr[1]);
    glDeleteBuffers(1, &BlockTexture);
}

GLuint BlockCatogary::getBlock(const int& index) const {
    return pPtr[index];
}

Block::Block():pHoverPlane(-1), pPosition(0, 0, 0) {

}
Block::~Block() {
}
char Block::getHoverPlane() const {
    return pHoverPlane;
}
void Block::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
}
void Block::setPosition(const glm::vec3& position) {
    pPosition = position;
}
void Block::setHoverPlane(const char& index) {
    pHoverPlane = index;
}
void Block::glDraw() const {
    glUseProgram(ShaderStorage::Default->CubeShader);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, PointSet::Default->BlockSet);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, BlockCatogary::Default->BlockTexture);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(1);

    GLuint originPoint;
    
    glGenBuffers(1, &originPoint);
    glBindBuffer(GL_UNIFORM_BUFFER, originPoint);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*3, &pPosition[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, originPoint);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, BlockCatogary::Default->getBlock(type));

    glDrawArrays(GL_QUADS, 0, 24);
    if (pHoverPlane!=-1) {
        glUseProgram(ShaderStorage::Default->MarginShader);
        glLineWidth(3);
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, PointSet::Default->MarginSet);
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

        glDrawArrays(GL_LINE_STRIP, 0, 16);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &margin);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &originPoint);
}