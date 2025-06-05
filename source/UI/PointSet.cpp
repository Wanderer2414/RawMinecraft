#include "PointSet.h"
#include "Global.h"
#include "tiny_gltf.h"
namespace MyBase3D {
    PointSet::PointSet() {

        glm::vec3 vertices[36];
        vertices[0] = {0, 0, 0};
        vertices[1] = {1, 0, 0};
        vertices[2] = {0, 0, 1};

        vertices[3] = {0, 0, 1};
        vertices[4] = {1, 0, 0};
        vertices[5] = {1, 0, 1};
    
        vertices[6] = {0, 0, 1};
        vertices[7] = {1, 0, 1};
        vertices[8] = {0, 1, 1};

        vertices[9] = {0, 1, 1};
        vertices[10] = {1, 0, 1};
        vertices[11] = {1, 1, 1};
    
        vertices[12] = {0, 1, 1};
        vertices[13] = {1, 1, 1};
        vertices[14] = {0, 1, 0};

        vertices[15] = {0, 1, 0};
        vertices[16] = {1, 1, 1};
        vertices[17] = {1, 1, 0};
    
        vertices[18] = {0, 1, 0};
        vertices[19] = {1, 1, 0};
        vertices[20] = {0, 0, 0};

        vertices[21] = {0, 0, 0};
        vertices[22] = {1, 1, 0};
        vertices[23] = {1, 0, 0};
    
        vertices[24] = {1, 1, 1};
        vertices[25] = {1, 0, 1};
        vertices[26] = {1, 1, 0};

        vertices[27] = {1, 1, 0};
        vertices[28] = {1, 0, 1};
        vertices[29] = {1, 0, 0};
    
        vertices[30] = {0, 0, 1};
        vertices[31] = {0, 1, 1};
        vertices[32] = {0, 0, 0};

        vertices[33] = {0, 0, 0};
        vertices[34] = {0, 1, 1};
        vertices[35] = {0, 1, 0};
    
        glGenBuffers(1, &__blockSet);
        glBindBuffer(GL_ARRAY_BUFFER, __blockSet);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*36*3, &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    
        vertices[0] = {0, 0, 0};
        vertices[1] = {16, 0, 0};
        vertices[2] = {16, 16, 0};
        vertices[3] = {0, 16, 0};
        vertices[4] = {0, 0, 0};
    
        vertices[5] = {0, 0, 16};
        vertices[6] = {16, 0, 16};
        vertices[7] = {16, 16, 16};
        vertices[8] = {0, 16, 16};
        vertices[9] = {0, 0, 16};
    
        vertices[10] = {16, 0, 16};
        vertices[11] = {16, 0, 0};
    
        vertices[12] = {16, 16, 0};
        vertices[13] = {16, 16, 16};
    
        vertices[14] = {0, 16, 16};
        vertices[15] = {0, 16, 0};
    
        glGenBuffers(1, &__chunkSet);
        glBindBuffer(GL_ARRAY_BUFFER, __chunkSet);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*16*3, &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
        vertices[0] = {0, 0, 0};
        vertices[1] = {1, 0, 0};
        vertices[2] = {1, 1, 0};
        vertices[3] = {0, 1, 0};
        vertices[4] = {0, 0, 0};
    
        vertices[5] = {0, 0, 1};
        vertices[6] = {1, 0, 1};
        vertices[7] = {1, 1, 1};
        vertices[8] = {0, 1, 1};
        vertices[9] = {0, 0, 1};
    
        vertices[10] = {1, 0, 1};
        vertices[11] = {1, 0, 0};
    
        vertices[12] = {1, 1, 0};
        vertices[13] = {1, 1, 1};
    
        vertices[14] = {0, 1, 1};
        vertices[15] = {0, 1, 0};
    
        glGenBuffers(1, &__marginSet);
        glBindBuffer(GL_ARRAY_BUFFER, __marginSet);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*16*3, &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    PointSet::~PointSet() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &__blockSet);
        glDeleteBuffers(1, &__chunkSet);
        glDeleteBuffers(1, &__marginSet);
    }
    GLuint PointSet::getBlockSet() const {
        return __blockSet;
    }
    GLuint PointSet::getChunkSet() const {
        return __chunkSet;
    }
    GLuint PointSet::getMarginSet() const {
        return __marginSet;
    }
}