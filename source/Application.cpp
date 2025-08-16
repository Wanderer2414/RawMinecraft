#include "Application.h"
#include "Block.h"
#include "ControlCenter.h"
#include "DrawingCenter.h"
#include "GameForm.h"
#include "IntroForm.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "ModelStorage.h"
#include "ShapeManager.h"

MyBase::ControlCenter* MyBase::ControlCenter::Default;
namespace MyCraft {
    Application::Application(const float& width, const float& height) 
 {
        auto& Default = MyBase::ControlCenter::Default = new MyBase::ControlCenter(width, height, "MyCraft");
        Default->OpenGLrequire(4, 6);
        Default->LimitFPS(0);
        __window = Default->InitWindow();
        Default->EnableTransparent();
    }
    Application::~Application() {
        MyCraft::DrawingCenter::close();
        MyBase::ShapeManager::close();
        MyBase3D::PointSet::close();
        MyBase3D::ShaderStorage::close();
        MyCraft::BlockCatogary::close();
        MyCraft::ModelStorage::close();
        MyBase::ControlCenter::Default->CloseWindow();
        delete MyBase::ControlCenter::Default;
    }

    void Application::run() {
        int formIndex = 0;
        while (!glfwWindowShouldClose(__window)) {
            switch (formIndex) {
                case 0: {
                    IntroForm introForm(__window, 0);
                    formIndex = introForm.run(__window);
                }
                break;
                case 1: {
                    GameForm gameForm(__window, 1);
                    formIndex = gameForm.run(__window);
                    // glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(myModel.scale));
                    // //shader.setMat4("model", modelMatrix);

                    // myModel.Draw();
                }
                break;
            }
        }
    }
};