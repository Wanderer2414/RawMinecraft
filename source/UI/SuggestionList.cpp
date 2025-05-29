#include "SuggestionList.h"
#include "Controller.h"
#include "Global.h"
#include "Textbox.h"
namespace MyBase {
    template <typename T>
    Suggestionlist<T>::Suggestionlist(ButtonSetting& b_setting, TextSetting& t_setting): Textbox<T>(b_setting, t_setting),
        __buttonSetting(&b_setting),
        __hoverIndex(-1),
        __autoComplete(false),
        __isDownward(true),
        _startShow(0), _endShow(0),
        __selectionIndex(0) {
    }
    template <typename T>
    Suggestionlist<T>::~Suggestionlist() {
    
    }
    template <typename T>
    unsigned int Suggestionlist<T>::getSelectionIndex() const {
        return __selectionIndex;
    }
    template <typename T>
    bool Suggestionlist<T>::contains(const sf::Vector2f& position) const {
        if (Textbox<T>::isFocus()) {
            sf::FloatRect rect;
            rect.left = Textbox<T>::getPosition().x;
            rect.top =  Textbox<T>::getPosition().y;
            rect.width = Textbox<T>::getSize().x;
            rect.height = (Textbox<T>::getSize().y+2)*std::min((size_t)6, __suggestionList.size()+1);
            return rect.contains(position);
        } else return Textbox<T>::contains(position);
    }
    template <typename T>
    bool Suggestionlist<T>::setHover(const sf::Vector2f& position) {
        bool ans = Controller::setHover(position);
        sf::Vector2f pos = position;
        if (Textbox<T>::isFocus()) {
            float offset = Textbox<T>::getSize().y + 2;
            pos.y -= Textbox<T>::getPosition().y;
            int start = floor(_startShow), end = ceil(_endShow);
            pos.y += (_startShow-start)*offset;
            int n;
            if (__isDownward) {
                n = pos.y/offset;
                pos.y -= n*offset;
                n+=start;
            }
            else {
                n = -pos.y/(Textbox<T>::getSize().y+2) + 1;
                pos.y += n*(Textbox<T>::getSize().y+2);
            }
            pos.y += Textbox<T>::getPosition().y;
            if (n>0 && Textbox<T>::contains(pos) && n<=__suggestionList.size()) {
                __hoverIndex = n-1;
            }
        }
        return ans;
    }
    template <typename T>
    std::size_t Suggestionlist<T>::getSelectionValue() const {
        if (__selectionIndex>=0 && __selectionIndex<__value.size()) return __value[__selectionIndex];
        else return 0;
    }
    template <typename T>
    void Suggestionlist<T>::setDirection(const bool& downward) {
        __isDownward = downward;
    }
    template <typename T>
    void Suggestionlist<T>::setAutoComplete(const bool& autocomplete) {
        __autoComplete = autocomplete;
    }
    template <typename T>
    void Suggestionlist<T>::setList(const std::vector<std::string>& list, const std::vector<std::size_t>& values) {
        __suggestionList = list;
        _endShow = std::min(list.size(), size_t(5));
        _startShow = 0;
        if (list.size() && __autoComplete) {
            if (__hoverIndex<0) __hoverIndex = 0;
            else if (__hoverIndex>=list.size()) __hoverIndex = list.size()-1;
        }
        else __hoverIndex = -1;
        __selectionIndex = list.size();
        __value = values;
    }
    template  <typename T> 
    void Suggestionlist<T>::reset() {
        Textbox<T>::reset();
        _wheelDelta = 0;
    }
    template <typename T>
    _catch_function(Suggestionlist<T>, CatchEvent) {
        bool is_changed = Textbox<T>::CatchEvent(window, event, state); 
        switch (event.type) {
        case sf::Event::KeyPressed: {
            if (Textbox<T>::isFocus()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                    __hoverIndex = (__hoverIndex+1)%__suggestionList.size();
                    is_changed = true;
                }
                else {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))  __hoverIndex = (__hoverIndex-1)%__suggestionList.size();
                    is_changed = true;
                }
                if (__hoverIndex!=-1) {
                    if (__hoverIndex>_endShow-1) {
                        _startShow += (__hoverIndex-_endShow)+1;
                        _endShow+=(__hoverIndex-_endShow)+1;
                    }
                    if (__hoverIndex<_startShow) {
                        _endShow+=(__hoverIndex-_startShow);
                        _startShow+=(__hoverIndex-_startShow);
                    }
                }
            }
            else if (Textbox<T>::_isEnter && __hoverIndex>=0 && __hoverIndex < __suggestionList.size()) {
                Textbox<T>::setString(__suggestionList[__hoverIndex]);
                __selectionIndex = __hoverIndex;
                __hoverIndex = -1;
                Textbox<T>::update();
                Textbox<T>::setFocus(false);
                is_changed = true;
            }
        }
            break;
        case sf::Event::MouseButtonPressed: {
            if (Textbox<T>::isHovered()) {
                if (__hoverIndex>=0 && __hoverIndex < __suggestionList.size()) {
                    Textbox<T>::_isEnter = true;
                    Textbox<T>::setFocus(false);
                    Textbox<T>::setString(__suggestionList[__hoverIndex]);
                    __selectionIndex = __hoverIndex;
                    __hoverIndex = -1;
                    Textbox<T>::setHover(false);
                    Textbox<T>::update();
                    is_changed = true;
                }
            }
        }
        break;
        case sf::Event::MouseWheelScrolled: {
            _wheelDelta = event.mouseWheelScroll.delta;
        }
        break;
        default:
            break;
        }
        return is_changed;
    }
    template<typename T>
    _catch_function(Suggestionlist<T>, AfterCatch) {
        bool is_changed = Textbox<T>::AfterCatch(window, event, state);
        if (Textbox<T>::isHovered()) {
            float delta = std::min(_wheelDelta*0.08f, __suggestionList.size()-1.f-_endShow);
            if (_endShow+delta+1<__suggestionList.size() && _startShow+delta >= 0) {
                _startShow += delta;
                _endShow += delta;
                is_changed = true;
            }
        }
        return is_changed;
    }
    template <typename T>
    void Suggestionlist<T>::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        Textbox<T>::draw(target, state);
        if (Textbox<T>::isFocus()) {
            T tmp = *(T*)this;
            tmp.setFillColor(__buttonSetting->normal_color);
            tmp.setOutlineColor(__buttonSetting->click_color);
            tmp.setOutlineThickness(2);
            sf::Text tmp_text = sf::Text(*this);
            float offset = Textbox<T>::getSize().y + 2;
            if (!__isDownward) offset = -offset;
            sf::RenderTexture texture;
            texture.create(T::getSize().x + T::getOutlineThickness()*2, offset*5);
            texture.clear(sf::Color::Transparent);
            texture.setView((sf::View)sf::FloatRect(T::getPosition().x-T::getOutlineThickness(), T::getPosition().y + offset, T::getSize().x + T::getOutlineThickness()*2, offset*5));
            
            int start = floor(_startShow), end = ceil(_endShow);
            tmp.move({0,(start-_startShow)*offset});
            tmp_text.move({0, (start-_startShow)*offset});
            for (int i = start; i<__suggestionList.size() && i<end; i++) {
                if (i==__hoverIndex) tmp.setFillColor(__buttonSetting->hover_color);
                tmp_text.move({tmp_text.getLocalBounds().width/2, 0});
                tmp_text.setString(__suggestionList[i]);
                tmp.move({0,offset});
    
                tmp_text.move({-tmp_text.getLocalBounds().width/2, offset});
                texture.draw(tmp, state);
                texture.draw(tmp_text, state);
                if (i==__hoverIndex) tmp.setFillColor(__buttonSetting->normal_color);
            }
            texture.display();
            sf::Sprite sprite(texture.getTexture());
            sprite.setPosition(T::getPosition().x - T::getOutlineThickness(), T::getPosition().y+offset);
            target.draw(sprite, state);
        }
    }
    
}