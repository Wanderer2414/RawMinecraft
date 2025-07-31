#include "PointSet.h"
#include "Global.h"
#include "tiny_gltf.h"
namespace MyBase3D {
    PointSet* PointSet::Default;
    PointSet::PointSet() {

        {
            glm::ivec4 vertices[36];
            vertices[0] = {0, 0, 0, 0};
            vertices[1] = {1, 0, 0, 0};
            vertices[2] = {1, 1, 0, 0};

            vertices[3] = {0, 0, 0, 0};
            vertices[4] = {1, 1, 0, 0};
            vertices[5] = {0, 1, 0, 0};

            vertices[6] = {0, 0, 0, 0};
            vertices[7] = {0, 0, 1, 0};
            vertices[8] = {1, 0, 1, 0};

            vertices[9] = {0, 0, 0, 0};
            vertices[10] = {1, 0, 1, 0};
            vertices[11] = {1, 0, 0, 0};

            vertices[12] = {0, 0, 0, 0};
            vertices[13] = {0, 0, 1, 0};
            vertices[14] = {0, 1, 1, 0};

            vertices[15] = {0, 0, 0, 0};
            vertices[16] = {0, 1, 1, 0};
            vertices[17] = {0, 1, 0, 0};
            //
            vertices[18] = {1, 1, 0, 0};
            vertices[19] = {1, 1, 1, 0};
            vertices[20] = {1, 0, 0, 0};

            vertices[21] = {1, 1, 1, 0};
            vertices[22] = {1, 0, 1, 0};
            vertices[23] = {1, 0, 0, 0};
            
            vertices[24] = {1, 1, 0, 0};
            vertices[25] = {1, 1, 1, 0};
            vertices[26] = {0, 1, 0, 0};
            
            vertices[27] = {1, 1, 1, 0};
            vertices[28] = {0, 1, 1, 0};
            vertices[29] = {0, 1, 0, 0};
            
            vertices[30] = {0, 1, 1, 0};
            vertices[31] = {1, 1, 1, 0};
            vertices[32] = {0, 0, 1, 0};

            vertices[33] = {1, 1, 1, 0};
            vertices[34] = {1, 0, 1, 0};
            vertices[35] = {0, 0, 1, 0};

            glGenBuffers(1, &__blockSet);
            glBindBuffer(GL_UNIFORM_BUFFER, __blockSet);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(int)*36*4, &vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
        {
            std::vector<unsigned int> marginIndices = {0, 1, 2, 5, 0, 7, 8, 19, 14,7, 8, 1, 2, 19, 14, 5, 0};
            marginIndices.resize(17*32);
            for (int i = 17; i<marginIndices.size(); i++) marginIndices[i] = marginIndices[i-17]+36;
            glGenBuffers(1, &__marginBlockIndices);
            glBindBuffer(GL_ARRAY_BUFFER, __marginBlockIndices);
            glBufferData(GL_ARRAY_BUFFER, marginIndices.size()*4,marginIndices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        {
            std::vector<int> ImageBlockIndices(36*32);
            for (int i = 0; i<ImageBlockIndices.size() ;i++) ImageBlockIndices[i] = i;
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
            #define row 6
            glm::vec2 tex_coord[37];
            tex_coord[0] = {3.0/18, 0};
            tex_coord[1] = {2.0/18, 0};
            tex_coord[2] = {2.0/18, 1.0/row};

            tex_coord[3] = {3.0/18, 0};
            tex_coord[4] = {2.0/18, 1.0/row};
            tex_coord[5] = {3.0/18, 1.0/row};
            
            tex_coord[6] = {6.0/18, 1.0/row};
            tex_coord[7] = {6.0/18, 0};
            tex_coord[8] = {5.0/18, 0};

            tex_coord[9] = {6.0/18, 1.0/row};
            tex_coord[10] = {5.0/18, 0};
            tex_coord[11] = {5.0/18, 1.0/row};
            
            tex_coord[12] = {3.0/18, 0};
            tex_coord[13] = {4.0/18, 0};
            tex_coord[14] = {4.0/18, 1.0/row};

            tex_coord[15] = {3.0/18, 0};
            tex_coord[16] = {4.0/18, 1.0/row};
            tex_coord[17] = {3.0/18, 1.0/row};
            //
            tex_coord[18] = {2.0/18, 1.0/row};
            tex_coord[19] = {1.0/18, 1.0/row};
            tex_coord[20] = {2.0/18, 0};

            tex_coord[21] = {1.0/18, 1.0/row};
            tex_coord[22] = {1.0/18, 0};
            tex_coord[23] = {2.0/18, 0};

            tex_coord[24] = {5.0/18, 1.0/row};
            tex_coord[25] = {5.0/18, 0};
            tex_coord[26] = {4.0/18, 1.0/row};

            tex_coord[27] = {5.0/18, 0};
            tex_coord[28] = {4.0/18, 0};
            tex_coord[29] = {4.0/18, 1.0/row};

            tex_coord[30] = {0, 1.0/row};
            tex_coord[31] = {1.0/18, 1.0/row};
            tex_coord[32] = {0, 0};

            tex_coord[33] = {1.0/18, 1.0/row};
            tex_coord[34] = {1.0/18, 0};
            tex_coord[35] = {0, 0};
            
            tex_coord[36] = {1.0/3, 1.0/row};
            glGenBuffers(1, &__blockUVS);
            glBindBuffer(GL_UNIFORM_BUFFER, __blockUVS);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*37*2, &tex_coord[0], GL_STATIC_DRAW);
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