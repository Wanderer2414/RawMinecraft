#include "Block.h"
#include "General.h"
#include "Global.h"
namespace MyCraft {
    BlockCatogary* BlockCatogary::Default;
    BlockCatogary::BlockCatogary() {
        __ptr.resize(3, 0);
        __ptr[0] = 0;
        __ptr[1] = MyBase::LoadTexture("assets/images/Dirt.png");
        __ptr[2] = MyBase::LoadTexture("assets/images/Grass.png");

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
    BlockCatogary& BlockCatogary::getInstance() {
        if (!Default) Default = new BlockCatogary();
        return *Default;
    }
    void BlockCatogary::close() {
        if (Default) {
            delete Default;
            Default = 0;
        }
    }
    GLuint MyCraft::BlockCatogary::getBlock(const int& index) const {
        return __ptr[index];
    }

    GLuint MyCraft::BlockCatogary::getTexCoord() const {
        return __blockTexture;
    }
}