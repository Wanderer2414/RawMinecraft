#include "Application.h"
#include "Block.h"
#include "ControlCenter.h"
#include "DrawingCenter.h"
#include "FlyweightStorage.h"
#include "IntroForm.h"
#include "MediateForm.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "ShapeManager.h"
#include "Sound.h"

namespace MyCraft {
    Application::Application(const float& width, const float& height) {
        MyBase::ControlCenter::getInstance().OpenGLrequire(4, 6);
        MyBase::ControlCenter::getInstance().LimitFPS(0);
        MyBase::ControlCenter::getInstance(). Maximize();
        MyBase::ControlCenter::getInstance(). EnableFullScreenMode();
        __window = MyBase::ControlCenter::getInstance().InitWindow(width, height, "MyCraft");
        MyBase::ControlCenter::getInstance().EnableTransparent();

        Sound::__backgroundMusic = new Sound("assets/sounds/BackgroundSound.mp3");
    }
    Application::~Application() {
        MyBase::ShapeManager::close();
        MyBase3D::PointSet::close();
        MyBase3D::ShaderStorage::close();
        DrawingCenter::Close();
        MyBase::FlyweightStorage::close();
        MyBase::ControlCenter::getInstance().CloseWindow();
        MyBase::ControlCenter::close();
        delete Sound::__backgroundMusic;
    }

    void Application::run() {
        int formIndex = 0;
        while (!glfwWindowShouldClose(__window) && formIndex>-1) {
            switch (formIndex) {
                case 0: {
                    IntroForm introForm(__window, 0);
                    formIndex = introForm.run(__window);
                }
                break;
                case 1: {
                    MediateForm mediateForm(__window, 1);
                    formIndex = mediateForm.run(__window);
                }
                break;
            }
        }
    }
};