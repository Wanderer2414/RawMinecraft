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
#include <ctime>

MyBase3D::PointSet* MyBase3D::PointSet::Default;
MyBase::ControlCenter* MyBase::ControlCenter::Default;
MyBase3D::ShaderStorage* MyBase3D::ShaderStorage::Default;
MyCraft::BlockCatogary* MyCraft::BlockCatogary::Default;
MyCraft::ModelStorage* MyCraft::ModelStorage::Default;
MyCraft::DrawingCenter* MyCraft::DrawingCenter::Default;
MyBase::ShapeManager* MyBase::ShapeManager::Default;
namespace MyCraft {
    Application::Application(const float& width, const float& height) {
        
        if (!glfwInit()) {
            std::cout << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
        __window = glfwCreateWindow(width, height, "MyCraft", nullptr, nullptr);
        if (!__window) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(__window);
        glfwSwapInterval(0);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        }
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        MyBase::ControlCenter::Default = new MyBase::ControlCenter(width, height, "MyCraft");
        MyBase3D::PointSet::Default = new MyBase3D::PointSet();
        MyBase3D::ShaderStorage::Default = new MyBase3D::ShaderStorage();
        MyCraft::BlockCatogary::Default = new MyCraft::BlockCatogary();
        MyCraft::ModelStorage::Default = new MyCraft::ModelStorage();
        MyCraft::DrawingCenter::Default = new MyCraft::DrawingCenter();
        MyBase::ShapeManager::Default = new MyBase::ShapeManager();
    }
    Application::~Application() {
        delete MyCraft::DrawingCenter::Default;
        delete MyBase::ControlCenter::Default;
        delete MyBase::ShapeManager::Default;
        delete MyBase3D::PointSet::Default;
        delete MyBase3D::ShaderStorage::Default;
        delete MyCraft::BlockCatogary::Default;
        delete MyCraft::ModelStorage::Default;
        glfwTerminate();
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
                }
                break;
            }
        }
    }
}