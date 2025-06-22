#include "PointSet.h"
#include "Global.h"
#include "tiny_gltf.h"
namespace MyBase3D {
    PointSet::PointSet() {

        glm::vec4 vertices[14];
        vertices[0] = {0, 0, 0, 0};
        vertices[1] = {1, 0, 0, 0};
        vertices[2] = {1, 1, 0, 0};
        vertices[3] = {0, 1, 0, 0};

        vertices[4] = {0, 0, 1, 0};
        vertices[5] = {1, 0, 1, 0};
        vertices[6] = {1, 1, 1, 0};
        vertices[7] = {0, 1, 1, 0};

        vertices[8] = {0, 0, 0, 0};
        vertices[9] = {1, 0, 0, 0};
        vertices[10] = {0, 0, 0, 0};
        vertices[11] = {1, 0, 0, 0};

        vertices[12] = {1, 1, 0, 0};
        vertices[13] = {0, 1, 0, 0};
    
        glGenBuffers(1, &__blockSet);
        glBindBuffer(GL_UNIFORM_BUFFER, __blockSet);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*14*4, &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        std::vector<unsigned int> marginIndices = {0, 1, 2, 3, 0, 4, 5, 6, 7,4, 5, 1, 2, 6, 7, 3, 0};
        marginIndices.resize(17*32);
        for (int i = 17; i<marginIndices.size(); i++) marginIndices[i] = marginIndices[i-17]+14;
        glGenBuffers(1, &__marginBlockIndices);
        glBindBuffer(GL_ARRAY_BUFFER, __marginBlockIndices);
        glBufferData(GL_ARRAY_BUFFER, marginIndices.size()*4,marginIndices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::vector<int> ImageBlockIndices = {0, 1, 4, 4, 1, 5, 4, 5, 7,7, 5, 6, 7, 6, 3, 3, 6, 2, 3, 2, 8, 8, 2, 9, 6, 5, 12,12, 5, 11,4,7,10,10,7,13};
        ImageBlockIndices.resize(36*32);
        for (int i = 36; i<ImageBlockIndices.size() ;i++) ImageBlockIndices[i] = ImageBlockIndices[i-36]+14;
        glGenBuffers(1, &__imageBlockIndices);
        glBindBuffer(GL_ARRAY_BUFFER, __imageBlockIndices);
        glBufferData(GL_ARRAY_BUFFER, ImageBlockIndices.size()*4,ImageBlockIndices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::vector<unsigned int> rectIndices = {0, 1,3, 3, 1, 2};
        glGenBuffers(1, &__rectangleIndices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __rectangleIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, rectIndices.size()*4, rectIndices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    PointSet::~PointSet() {
        glDeleteBuffers(1, &__blockSet);
        glDeleteBuffers(1, &__marginBlockIndices);
        glDeleteBuffers(1, &__imageBlockIndices);
        glDeleteBuffers(1, &__rectangleIndices);
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
}