#include "HitBoxCenter.h"
#include "Block.h"
#include "Command.h"
#include "Controller.h"
#include "General.h"
#include "Global.h"
#include "Model.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "World.h"
namespace MyCraft {
    
    HitBoxCenter::HitBoxCenter() {
        float colors[24];
        for (int i = 0; i<8; i++) {
            colors[3*i] = 1;
            colors[3*i+1] = 0;
            colors[3*i+2] = 0;
        }
        glGenBuffers(1, &__colors);
        glBindBuffer(GL_ARRAY_BUFFER, __colors);
        glBufferData(GL_ARRAY_BUFFER, 96, colors, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    HitBoxCenter::~HitBoxCenter() {
        glDeleteBuffers(1, &__colors);
    }
    void HitBoxCenter::setWorld(World* world) {
        __world = world;
    }
    void HitBoxCenter::insert(Model* model) {
        __models.push_back(model);
    }
    void HitBoxCenter::erase(Model* model) {
    }
    bool HitBoxCenter::handle(GLFWwindow* window) {
        bool is_changed = Controller3D::handle(window);
        //Receive request
        for (auto& model : __models) {
            if (model->request.size()) {
                std::queue<float>& q = model->request;
                int sz = q.size();
                glm::mat4x3 shape = model->getShape();
                if (q.front() == Command::MoveRequest) {
                    q.pop();
                    glm::vec3 dir;
                    dir.x = q.front(); q.pop();
                    dir.y = q.front(); q.pop();
                    dir.z = 0;
                    bool res = true;
                    glm::vec3 npos = shape[0] + dir, epos = npos + shape[1];
                    std::queue<glm::vec3> q = rasterize(npos, epos);
                    while (q.size() && res) {
                        if (__world->at(q.front()).getType() != BlockCatogary::Air) res = false;
                        q.pop();
                    }
                    if (!res) {
                        glm::vec3 npos = shape[0] + glm::vec3(dir.x, 0, 0), epos = npos + shape[1];
                        std::queue<glm::vec3> q = rasterize(npos, epos);
                        res = true;
                        while (q.size() && res) {
                            if (__world->at(q.front()).getType() != BlockCatogary::Air) res = false;
                            q.pop();
                        }
                        if (res) dir.y = 0;
                        else {
                            glm::vec3 npos = shape[0] + glm::vec3(0, dir.y, 0), epos = npos + shape[1];
                            std::queue<glm::vec3> q = rasterize(npos, epos);
                            res = true;
                            while (q.size() && res) {
                                if (__world->at(q.front()).getType() != BlockCatogary::Air) res = false;
                                q.pop();
                            }
                            if (res) dir.x = 0;
                        }
                    }
                    if (res) {
                        model->post.push(Command::MoveRequest);
                        model->post.push(dir.x);
                        model->post.push(dir.y);
                    }
                } 
            }
        }
        return false;
    }
    void HitBoxCenter::glDraw() const {
        glUseProgram(MyBase3D::ShaderStorage::Default->GetDefaultShader());
        for (auto& model: __models) {
            glm::mat4x3 mat = model->getShape();
            glm::vec3 shape[8] = {mat[0], mat[0]+mat[1], mat[0] + mat[1]+ mat[2], mat[0] +mat[2]};
            for (int i = 4; i<8; i++) shape[i] = shape[i-4]+mat[3];

            GLuint VAO, VBO;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 96, &shape, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);

            glBindBuffer(GL_ARRAY_BUFFER, __colors);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::Default->getMarginBlockIndices());

            
            glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_INT, 0);

            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
        }
    }
}