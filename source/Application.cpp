#include "Application.h"
#include "Block.h"
#include "GLFW/glfw3.h"
#include "InfoCenter.h"
#include "GameForm.h"
#include "Model.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "ModelStorage.h"

MyBase3D::PointSet* MyBase3D::PointSet::Default;
MyCraft::InfoCenter* MyCraft::InfoCenter::Default;
MyBase3D::ShaderStorage* MyBase3D::ShaderStorage::Default;
MyCraft::BlockCatogary* MyCraft::BlockCatogary::Default;
MyCraft::ModelStorage* MyCraft::ModelStorage::Default;
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
        if (!gladLoadGL() ) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        }
        glEnable(GL_DEPTH_TEST);

        InfoCenter::Default = new InfoCenter(width, height);
        MyBase3D::PointSet::Default = new MyBase3D::PointSet();
        MyBase3D::ShaderStorage::Default = new MyBase3D::ShaderStorage();
        MyCraft::BlockCatogary::Default = new MyCraft::BlockCatogary();
        MyCraft::ModelStorage::Default = new MyCraft::ModelStorage();
    }
    Application::~Application() {
        delete InfoCenter::Default;
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
                    GameForm gameForm(__window, 0);
                    gameForm.run(__window);
                }
                break;
            }
        }
    }
}