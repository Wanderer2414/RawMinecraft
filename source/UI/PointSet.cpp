#include "PointSet.h"
#include "Global.h"
#include "tiny_gltf.h"
namespace MyBase3D {
    PointSet* PointSet::Default;
    PointSet::PointSet() {

        glm::ivec4 vertices[18];
        vertices[0] = {0, 0, 0, 0};
        vertices[1] = {1, 0, 0, 0};
        vertices[2] = {1, 1, 0, 0};
        vertices[3] = {0, 1, 0, 0};

        vertices[4] = {0, 0, 1, 0};
        vertices[5] = {1, 0, 1, 0};
        vertices[6] = {1, 1, 1, 0};
        vertices[7] = {0, 1, 1, 0};

        vertices[8] = {0, 0, 1, 0};
        vertices[9] = {0, 1, 1, 0};

        vertices[10] = {0, 1, 0, 0};
        vertices[11] = {1, 1, 0, 0};
        vertices[12] = {1, 1, 1, 0};
        vertices[13] = {0, 1, 1, 0};

        vertices[14] = {0, 0, 0, 0};
        vertices[15] = {1, 0, 0, 0};
        vertices[16] = {1, 0, 1, 0};
        vertices[17] = {0, 0, 1, 0};
        {
            glGenBuffers(1, &__blockSet);
            glBindBuffer(GL_UNIFORM_BUFFER, __blockSet);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(int)*18*4, &vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
        {
            std::vector<unsigned int> marginIndices = {0, 1, 2, 3, 0, 4, 5, 6, 7,4, 5, 1, 2, 6, 7, 3, 0};
            marginIndices.resize(17*32);
            for (int i = 17; i<marginIndices.size(); i++) marginIndices[i] = marginIndices[i-17]+14;
            glGenBuffers(1, &__marginBlockIndices);
            glBindBuffer(GL_ARRAY_BUFFER, __marginBlockIndices);
            glBufferData(GL_ARRAY_BUFFER, marginIndices.size()*4,marginIndices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        {
            std::vector<int> ImageBlockIndices = {7, 4, 6, 6, 4, 5, 6, 5, 2,2, 5, 1, 2, 1, 3, 3, 1, 0, 3, 0, 9, 9, 0, 8, 10, 13, 11, 11, 13, 12, 15, 16, 14, 14, 16, 17};
            ImageBlockIndices.resize(36*32);
            for (int i = 36; i<ImageBlockIndices.size() ;i++) ImageBlockIndices[i] = ImageBlockIndices[i-36]+18;
            glGenBuffers(1, &__imageBlockIndices);
            glBindBuffer(GL_ARRAY_BUFFER, __imageBlockIndices);
            glBufferData(GL_ARRAY_BUFFER, ImageBlockIndices.size()*4,ImageBlockIndices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        {
            std::vector<unsigned int> rectIndices = {0, 1,3, 3, 1, 2};
            glGenBuffers(1, &__rectangleIndices);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __rectangleIndices);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, rectIndices.size()*4, rectIndices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        {
            std::vector<glm::vec2> rectSet = {{0,0}, {0,1}, {1,1}, {1, 0}};
            glGenBuffers(1, &__rectangle2DSet);
            glBindBuffer(GL_ARRAY_BUFFER, __rectangle2DSet);
            glBufferData(GL_ARRAY_BUFFER, rectSet.size()*sizeof(glm::vec2), rectSet.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glGenBuffers(1, &__rectangle2DUV);
            glBindBuffer(GL_ARRAY_BUFFER, __rectangle2DUV);
            glBufferData(GL_ARRAY_BUFFER, rectSet.size()*sizeof(glm::vec2), rectSet.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        {
            glm::vec2 tex_coord[19] = {{3.0/18, 0},{2.0/18, 0},{2.0/18, 1.0/4},
                                        {3.0/18, 1.0/4},{0, 0},{1.0/18, 0},
                                        {1.0/18, 1.0/4},{0, 1.0/4},{4.0/18, 0},
                                        {4.0/18, 1.0/4},{4.0/18, 1.0/4},{5.0/18, 1.0/4},
                                        {5.0/18, 0},{4.0/18, 0},{6.0/18, 1.0/4},
                                        {5.0/18, 1.0/4},{5.0/18, 0},{6.0/18, 0}};
            tex_coord[18] = {1.0/3, 1.0/4};
            glGenBuffers(1, &__blockUVS);
            glBindBuffer(GL_UNIFORM_BUFFER, __blockUVS);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*19*2, &tex_coord[0], GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }
    PointSet::~PointSet() {
        glDeleteBuffers(1, &__blockSet);
        glDeleteBuffers(1, &__blockUVS);
        glDeleteBuffers(1, &__marginBlockIndices);
        glDeleteBuffers(1, &__imageBlockIndices);
        glDeleteBuffers(1, &__rectangleIndices);
        glDeleteBuffers(1, &__rectangle2DSet);
        glDeleteBuffers(1, &__rectangle2DUV);
    }

    PointSet& PointSet::getInstance() {
        if (!Default) Default = new PointSet();
        return *Default;
    }
    void PointSet::close() {
        if (Default) {
            delete Default;
            Default = 0;
        }
    }
    GLuint PointSet::getBlockUVS() const {
        return __blockUVS;
    }
    GLuint PointSet::getBlockSet() const {
        return __blockSet;
    }
    GLuint PointSet::getImageBlockIndices() const {
        return __imageBlockIndices;
    }
    GLuint PointSet::getMarginBlockIndices() const {
        return __marginBlockIndices;
    }
    GLuint PointSet::getRectangleIndices() const {
        return __rectangleIndices;
    }
    GLuint PointSet::getRectangle2DSet() const {
        return __rectangle2DSet;
    }
    GLuint PointSet::getRectangle2DUV() const {
        return __rectangle2DUV;
    }
}