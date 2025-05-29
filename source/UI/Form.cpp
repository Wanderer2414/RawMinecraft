#include "Form.h"
#include "Container.h"
#include "Controller.h"
#include "Global.h"
#include <GL/gl.h>

extern sf::Vector2f WindowSize;
namespace MyBase {
    Form::Form(const int& index) {
        _formIndex = index;
        _returnValue = INT_MIN;
    }
    Form::~Form() {
    
    }
    sf::Vector2f Form::getSize() const {
        return WindowSize;
    }
    _catch_function(Form, CatchEvent) {
        bool ans = false;
        if (event.type == sf::Event::Resized) {
            sf::FloatRect rect = {0, 0, 1.0f*event.size.width, 1.0f*event.size.height};
            window.setView(sf::View(rect));
            ans = true;
        }
        ans = Container::CatchEvent(window, event, state) || ans;
        return ans;
    }
    bool Form::contains(const sf::Vector2f& position) const {
        return true;
    }
    int Form::run(sf::RenderWindow& window) {
        sf::Event event;
        bool is_changed = true, is_catched = false;
        while (window.isOpen()) {
            reset();
            while (window.pollEvent(event)) {
                if (!is_catched) {
                    is_catched = true;
                    is_changed = BeforeCatch(window, event) || is_changed;
                    is_changed = setHover(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) || is_changed;
                }
                is_changed = CatchEvent(window, event) || is_changed;
                if (event.type == sf::Event::Closed) window.close();
            }
            if (is_catched) {
                is_changed = AfterCatch(window, event) || is_changed;
                is_catched = false;
            }
            is_changed = handle(window) || is_changed;
            if (is_changed) {
                window.clear();
                draw(window);
                window.display();
            }
            if (_returnValue!=INT_MIN) return _returnValue;
            is_changed = 0;
        }
        return _formIndex;
    }
    void Form::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        for (const auto& [child, layer]:children) 
            if (_currentFocus == -1 || child != children[_currentFocus].first) target.draw(*child, state);
        if (_currentFocus != -1) target.draw(*children[_currentFocus].first, state);
    }
}