#include "PointSet.h"
#include "Global.h"
    
PointSet::PointSet() {

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

    glGenBuffers(1, &BlockSet);
    glBindBuffer(GL_ARRAY_BUFFER, BlockSet);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*24*3, &vertices[0], GL_STATIC_DRAW);
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

    glGenBuffers(1, &ChunkSet);
    glBindBuffer(GL_ARRAY_BUFFER, ChunkSet);
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

    glGenBuffers(1, &MarginSet);
    glBindBuffer(GL_ARRAY_BUFFER, MarginSet);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*16*3, &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
PointSet::~PointSet() {
    glDeleteBuffers(1, &BlockSet);
    glDeleteBuffers(1, &ChunkSet);
    glDeleteBuffers(1, &MarginSet);
}
