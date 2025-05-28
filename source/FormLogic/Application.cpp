#include "Application.h"
#include "Block.h"
#include "GameForm.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include <GL/gl.h>

Vector2f WindowSize;
MyBase3D::ShaderStorage*    MyBase3D::ShaderStorage::Default = 0;
MyBase3D::PointSet*         MyBase3D::PointSet::Default = 0;
MyCraft::BlockCatogary*     MyCraft::BlockCatogary::Default = 0;

namespace MyBase {

    Application::Application(const Vector2f& window_size, const string& title) {
        ContextSettings settings;
        settings.depthBits = 24; 
        settings.stencilBits = 8;
        settings.antialiasingLevel = 4;
        settings.majorVersion = 3;
        settings.minorVersion = 3;
        settings.attributeFlags = ContextSettings::Default;
        WindowSize = window_size ;
    
        window.create(VideoMode(window_size.x, window_size.y), title, Style::Default, settings);
        Mouse::setPosition(static_cast<Vector2i>(window.getPosition() + static_cast<Vector2i>(WindowSize)/2), window);
        window.display();
    
        gladLoadGL();
        
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
    
        MyBase3D::ShaderStorage::Default = new MyBase3D::ShaderStorage();
        MyCraft::BlockCatogary::Default = new MyCraft::BlockCatogary();
        MyBase3D::PointSet::Default = new MyBase3D::PointSet();
    }
    Application::~Application() {
        delete MyBase3D::ShaderStorage::Default;
        MyBase3D::ShaderStorage::Default = 0;
        delete MyCraft::BlockCatogary::Default;
        MyCraft::BlockCatogary::Default = 0;
    }
    void Application::run() {
        int form_index = 0;
        while (window.isOpen()) {
            switch (form_index) {
                case 0: {
                    MyCraft::GameForm gameForm(window, 0);
                    form_index = gameForm.run(window);
                };
                break;
            }
        }
    }
}