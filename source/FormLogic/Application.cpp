#include "Application.h"
#include "Block.h"
#include "GameForm.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include <GL/gl.h>

sf::Vector2f WindowSize;
MyBase3D::ShaderStorage*    MyBase3D::ShaderStorage::Default = 0;
MyBase3D::PointSet*         MyBase3D::PointSet::Default = 0;
MyCraft::BlockCatogary*     MyCraft::BlockCatogary::Default = 0;

namespace MyBase {

    Application::Application(const sf::Vector2f& window_size, const std::string& title) {
        sf::ContextSettings settings;
        settings.depthBits = 24; 
        settings.stencilBits = 8;
        settings.antialiasingLevel = 4;
        settings.majorVersion = 3;
        settings.minorVersion = 3;
        settings.attributeFlags = sf::ContextSettings::Default;
        WindowSize = window_size ;
    
        __window.create(sf::VideoMode(window_size.x, window_size.y), title, sf::Style::Default, settings);
        sf::Mouse::setPosition(static_cast<sf::Vector2i>(__window.getPosition() + static_cast<sf::Vector2i>(WindowSize)/2), __window);
        __window.display();
    
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
        int formIndex = 0;
        while (__window.isOpen()) {
            switch (formIndex) {
                case 0: {
                    MyCraft::GameForm gameForm(__window, 0);
                    formIndex = gameForm.run(__window);
                };
                break;
            }
        }
    }
}