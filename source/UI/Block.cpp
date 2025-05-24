#include "Block.h"
#include "General.h"
#include "Global.h"
#include "ShaderStorage.h"
#include "glm/fwd.hpp"
#include <GL/gl.h>

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

    glm::vec3 vertices[24];
    vertices[0] = {0, 0, 0};
    vertices[1] = {1, 0, 0};
    vertices[2] = {1, 0, 1};
    vertices[3] = {0, 0, 1};

    vertices[4] = {0, 0, 1};
    vertices[5] = {1, 0, 1};
    vertices[6] = {1, 1, 1};
    vertices[7] = {0, 1, 1};

    vertices[8] = {0, 1, 1};
    vertices[9] = {1, 1, 1};
    vertices[10] = {1, 1, 0};
    vertices[11] = {0, 1, 0};

    vertices[12] = {0, 1, 0};
    vertices[13] = {1, 1, 0};
    vertices[14] = {1, 0, 0};
    vertices[15] = {0, 0, 0};

    vertices[16] = {1, 1, 1};
    vertices[17] = {1, 0, 1};
    vertices[18] = {1, 0, 0};
    vertices[19] = {1, 1, 0};

    vertices[20] = {0, 0, 1};
    vertices[21] = {0, 1, 1};
    vertices[22] = {0, 1, 0};
    vertices[23] = {0, 0, 0};

    glGenBuffers(1, &BlockDesign);
    glBindBuffer(GL_ARRAY_BUFFER, BlockDesign);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*24*3, &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    glDeleteBuffers(1, &BlockDesign);
    glDeleteBuffers(1, &BlockTexture);
}

GLuint BlockCatogary::getBlock(const int& index) const {
    return pPtr[index];
}

Block::Block() {
    pPosition = {0,0,0};

}
Block::~Block() {
}
bool Block::setHover(const Ray3f& sight) {
    bool hover = false;
    return Controller3D::setHover(hover);
}
void Block::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
}
void Block::setPosition(const glm::vec3& position) {
    pPosition = position;
}
bool Block::contains(const Ray3f& point) const  {
    return false;
}
void Block::draw(RenderTarget& target, RenderStates state) const {
}
void Block::glDraw() const {
    glUseProgram(ShaderStorage::Default->CubeShader);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, BlockCatogary::Default->BlockDesign);
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
    glBindTexture(GL_TEXTURE_2D, BlockCatogary::Default->getBlock(1));

    glDrawArrays(GL_QUADS, 0, 24);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &originPoint);
    
    // if (isFocus()) glColor3f(1, 0, 0);
    // else glColor3f(1, 1, 1);
    // glColor3f(1, 1, 1);
    // glBindTexture(GL_TEXTURE_2D, (*pBlockCatogary)[type]);
    // for (int i = 0; i<6; i++) {
    //     glBegin(GL_QUADS);
    //         glTexCoord2f(1.f/3, 0);
    //         glVertex3f(pPlane[i][0]);
    //         glTexCoord2f(1.f/3, 1.f/3);
    //         glVertex3f(pPlane[i][1]);
    //         glTexCoord2f(2.f/3, 1.f/3);
    //         glVertex3f(pPlane[i][2]);
    //         glTexCoord2f(2.f/3, 0);
    //         glVertex3f(pPlane[i][3]);
    //     glEnd();
    // }
    // if (isHovered()) {
    //     glColor3f(0.1,0.1,0.1);
    //     glLineWidth(2);
    //     for (int i = 0; i<6; i++) {
    //         glBegin(GL_LINE_STRIP);
    //             glVertex3f(pPlane[i][0]);
    //             glVertex3f(pPlane[i][1]);
    //             glVertex3f(pPlane[i][2]);
    //             glVertex3f(pPlane[i][3]);
    //             glVertex3f(pPlane[i][0]);
    //         glEnd();
    //     }
    // }
}