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
            std::queue<float>& q = model->request;
            while (q.size()) {
                glm::mat4x3 shape = model->getShape();
                if (q.front() == Command::MoveRequest) {
                    q.pop();
                    glm::vec3 dir;
                    dir.x = q.front(); q.pop();
                    dir.y = q.front(); q.pop();
                    dir.z = 0;
                    bool below_result = true, above_result = true;
                    //Below check
                    glm::vec3 npos = shape[0] + dir, epos = npos + shape[1];
                    std::queue<glm::vec3> q = rasterize(npos, epos);
                    while (q.size() && below_result) {
                        if (__world->at(q.front()).getType() != BlockCatogary::Air) below_result = false;
                        q.pop();
                    }
                    //Above block check
                    npos.z += 1;
                    epos.z += 1;
                    q = rasterize(npos, epos);
                    while (q.size() && above_result) {
                        if (__world->at(q.front()).getType() != BlockCatogary::Air) above_result = false;
                        q.pop();
                    }
                    if (!below_result || !above_result) {
                        //Check auto jump
                        if (above_result) {
                            below_result = above_result = true;
                            q = rasterize(shape[0]-glm::vec3(0,0,1), shape[0]+shape[1]-glm::vec3(0,0,1));
                            while (q.size() && above_result) {
                                if (__world->at(q.front()).getType() == BlockCatogary::Air) below_result = false;
                                q.pop();
                            }
                            q = rasterize(shape[0]+shape[2]-glm::vec3(0,0,1), shape[0]+shape[1]+shape[2]-glm::vec3(0,0,1));
                            while (q.size() && above_result) {
                                if (__world->at(q.front()).getType() == BlockCatogary::Air) below_result = false;
                                q.pop();
                            }
                            npos.z += 1;
                            epos.z += 1;
                            q = rasterize(npos, epos);
                            while (q.size() && above_result) {
                                if (__world->at(q.front()).getType() != BlockCatogary::Air) above_result = false;
                                q.pop();
                            }
                        }
                        if (below_result && above_result) dir.z=1;
                        else {
                            //Check parallel Ox
                            npos = shape[0] + glm::vec3(dir.x, 0, 0); epos = npos + shape[1];
                            below_result = above_result = true;
                            q = rasterize(npos, epos);
                            while (q.size() && below_result) {
                                if (__world->at(q.front()).getType() != BlockCatogary::Air) below_result = false;
                                q.pop();
                            }
                            npos.z++; epos.z++;
                            q = rasterize(npos, epos);
                            while (q.size() && below_result) {
                                if (__world->at(q.front()).getType() != BlockCatogary::Air) above_result = false;
                                q.pop();
                            }

                            if (above_result && below_result) dir.y = 0;
                            else {
                                //Check paralel Oy
                                npos = shape[0] + glm::vec3(0, dir.y, 0); epos = npos + shape[1];
                                below_result = above_result = true;
                                q = rasterize(npos, epos);
                                while (q.size() && below_result) {
                                    if (__world->at(q.front()).getType() != BlockCatogary::Air) below_result = false;
                                    q.pop();
                                }
                                npos.z++; epos.z++;
                                q = rasterize(npos, epos);
                                while (q.size() && below_result) {
                                    if (__world->at(q.front()).getType() != BlockCatogary::Air) below_result = false;
                                    q.pop();
                                }
                                if (below_result) dir.x = 0;
                            }
                        }
                    }
                    if (below_result) {
                        model->post.push(Command::MoveRequest);
                        model->post.push(dir.x);
                        model->post.push(dir.y);
                        model->post.push(dir.z);

                        shape[3] = shape[0] + shape[2];
                        shape[3][2]--;
                        shape[2] += shape[0] + shape[1];
                        shape[2][2]--;
                        shape[1] += shape[0];
                        shape[1][2]--;
                        shape[0][2]--;

                        if (__world->at(shape[0]).getType()==BlockCatogary::Air && 
                            __world->at(shape[1]).getType()==BlockCatogary::Air && 
                            __world->at(shape[2]).getType()==BlockCatogary::Air && 
                            __world->at(shape[3]).getType()==BlockCatogary::Air) {
                                model->post.push(Command::FallPost);
                                model->post.push(-0.3);
                        }
                        model->update();
                        is_changed = true;
                    }
                } 
                else if (q.front() == Command::FallRequest) {
                    q.pop();
                    float z = q.front(); q.pop();
                    if (z<=0) {
                        z -= 0.06;
                        bool isFall = true;
                        shape[3] = shape[0] + shape[2];
                        isFall = isFall && (__world->at(shape[3]+glm::vec3(0,0,z)).getType()==BlockCatogary::Air || 
                                            __world->at(shape[3]).getType()!=BlockCatogary::Air); 
                        shape[2] += shape[0] + shape[1];
                        isFall = isFall && (__world->at(shape[2]+glm::vec3(0,0,z)).getType()==BlockCatogary::Air || 
                                            __world->at(shape[2]).getType()!=BlockCatogary::Air); 
                        shape[1] += shape[0];
                        isFall = isFall && (__world->at(shape[1]+glm::vec3(0,0,z)).getType()==BlockCatogary::Air || 
                                            __world->at(shape[1]).getType()!=BlockCatogary::Air); 

                        isFall = isFall && (__world->at(shape[0]+glm::vec3(0,0,z)).getType()==BlockCatogary::Air || 
                                            __world->at(shape[0]).getType()!=BlockCatogary::Air); 

                        if (isFall) {
                                model->post.push(Command::FallRequest);
                                model->post.push(z);
                                model->update();
                        }
                        else {
                            float delta = shape[0][2] - floor(shape[0][2]);
                            model->post.push(Command::FallRequest);
                            model->post.push(-delta);
                            model->post.push(Command::StopFallPost);
                            model->update();
                        }
                    }
                    else if (z>0) {
                        shape[0] += shape[3];

                        shape[3] = shape[0] + shape[2];
                        shape[3][2]++;
                        shape[2] += shape[0] + shape[1];
                        shape[2][2]++;
                        shape[1] += shape[0];
                        shape[1][2]++;
                        shape[0][2]++;
                        if (__world->at(shape[0]).getType()==BlockCatogary::Air && 
                            __world->at(shape[1]).getType()==BlockCatogary::Air && 
                            __world->at(shape[2]).getType()==BlockCatogary::Air && 
                            __world->at(shape[3]).getType()==BlockCatogary::Air) {
                                model->post.push(Command::FallRequest);
                                model->post.push(z-0.035);
                                model->update();
                        }
                        else {
                            float delta = floor(shape[0][2]) - shape[0][2] + 1 - 0.1;
                            model->post.push(Command::FallRequest);
                            model->post.push(delta);
                            model->update();
                        }
                    }
                }
                else q.pop();
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