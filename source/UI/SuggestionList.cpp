#include "SuggestionList.h"
#include "Controller.h"
#include "Global.h"
#include "Textbox.h"

template <typename T>
Suggestionlist<T>::Suggestionlist(ButtonSetting& b_setting, TextSetting& t_setting): Textbox<T>(b_setting, t_setting) {
    button_setting = &b_setting;
    m_hover_index = -1;
    m_autocomplete = false;
    is_downward = true;
    start_show = end_show = 0;
    m_selection_index = 0;
}
template <typename T>
Suggestionlist<T>::~Suggestionlist() {

}
template <typename T>
unsigned int Suggestionlist<T>::getSelectionIndex() const {
    return m_selection_index;
}
template <typename T>
bool Suggestionlist<T>::contains(const Vector2f& position) const {
    if (Textbox<T>::isFocus()) {
        FloatRect rect;
        rect.left = Textbox<T>::getPosition().x;
        rect.top =  Textbox<T>::getPosition().y;
        rect.width = Textbox<T>::getSize().x;
        rect.height = (Textbox<T>::getSize().y+2)*min((size_t)6, m_suggestion_list.size()+1);
        return rect.contains(position);
    } else return Textbox<T>::contains(position);
}
template <typename T>
bool Suggestionlist<T>::setHover(const Vector2f& position) {
    bool ans = Controller::setHover(position);
    Vector2f pos = position;
    if (Textbox<T>::isFocus()) {
        float offset = Textbox<T>::getSize().y + 2;
        pos.y -= Textbox<T>::getPosition().y;
        int start = floor(start_show), end = ceil(end_show);
        pos.y += (start_show-start)*offset;
        int n;
        if (is_downward) {
            n = pos.y/offset;
            pos.y -= n*offset;
            n+=start;
        }
        else {
            n = -pos.y/(Textbox<T>::getSize().y+2) + 1;
            pos.y += n*(Textbox<T>::getSize().y+2);
        }
        pos.y += Textbox<T>::getPosition().y;
        if (n>0 && Textbox<T>::contains(pos) && n<=m_suggestion_list.size()) {
            m_hover_index = n-1;
        }
    }
    return ans;
}
template <typename T>
size_t Suggestionlist<T>::getSelectionValue() const {
    if (m_selection_index>=0 && m_selection_index<value.size()) return value[m_selection_index];
    else return 0;
}
template <typename T>
void Suggestionlist<T>::setDirection(const bool& downward) {
    is_downward = downward;
}
template <typename T>
void Suggestionlist<T>::setAutoComplete(const bool& autocomplete) {
    m_autocomplete = autocomplete;
}
template <typename T>
void Suggestionlist<T>::setList(const vector<string>& list, const vector<size_t>& values) {
    m_suggestion_list = list;
    end_show = min(list.size(), size_t(5));
    start_show = 0;
    if (list.size() && m_autocomplete) {
        if (m_hover_index<0) m_hover_index = 0;
        else if (m_hover_index>=list.size()) m_hover_index = list.size()-1;
    }
    else m_hover_index = -1;
    m_selection_index = list.size();
    value = values;
}
template  <typename T> 
void Suggestionlist<T>::reset() {
    Textbox<T>::reset();
    wheel_delta = 0;
}
template <typename T>
_catch_function(Suggestionlist<T>, CatchEvent) {
    bool is_changed = Textbox<T>::CatchEvent(window, event, state); 
    switch (event.type) {
    case Event::KeyPressed: {
        if (Textbox<T>::isFocus()) {
            if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
                m_hover_index = (m_hover_index+1)%m_suggestion_list.size();
                is_changed = true;
            }
            else {
                if (Keyboard::isKeyPressed(Keyboard::Key::Up))  m_hover_index = (m_hover_index-1)%m_suggestion_list.size();
                is_changed = true;
            }
            if (m_hover_index!=-1) {
                if (m_hover_index>end_show-1) {
                    start_show += (m_hover_index-end_show)+1;
                    end_show+=(m_hover_index-end_show)+1;
                }
                if (m_hover_index<start_show) {
                    end_show+=(m_hover_index-start_show);
                    start_show+=(m_hover_index-start_show);
                }
            }
        }
        else if (Textbox<T>::m_is_enter && m_hover_index>=0 && m_hover_index < m_suggestion_list.size()) {
            Textbox<T>::setString(m_suggestion_list[m_hover_index]);
            m_selection_index = m_hover_index;
            m_hover_index = -1;
            Textbox<T>::update();
            Textbox<T>::setFocus(false);
            is_changed = true;
        }
    }
        break;
    case Event::MouseButtonPressed: {
        if (Textbox<T>::isHovered()) {
            if (m_hover_index>=0 && m_hover_index < m_suggestion_list.size()) {
                Textbox<T>::m_is_enter = true;
                Textbox<T>::setFocus(false);
                Textbox<T>::setString(m_suggestion_list[m_hover_index]);
                m_selection_index = m_hover_index;
                m_hover_index = -1;
                Textbox<T>::setHover(false);
                Textbox<T>::update();
                is_changed = true;
            }
        }
    }
    break;
    case Event::MouseWheelScrolled: {
        wheel_delta = event.mouseWheelScroll.delta;
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
        float delta = min(wheel_delta*0.08f, m_suggestion_list.size()-1.f-end_show);
        if (end_show+delta+1<m_suggestion_list.size() && start_show+delta >= 0) {
            start_show += delta;
            end_show += delta;
            is_changed = true;
        }
    }
    return is_changed;
}
template <typename T>
void Suggestionlist<T>::draw(RenderTarget& target, RenderStates state) const {
    Textbox<T>::draw(target, state);
    if (Textbox<T>::isFocus()) {
        T tmp = *(T*)this;
        tmp.setFillColor(button_setting->normal_color);
        tmp.setOutlineColor(button_setting->click_color);
        tmp.setOutlineThickness(2);
        Text tmp_text = Text(*this);
        float offset = Textbox<T>::getSize().y + 2;
        if (!is_downward) offset = -offset;
        RenderTexture texture;
        texture.create(T::getSize().x + T::getOutlineThickness()*2, offset*5);
        texture.clear(Color::Transparent);
        texture.setView((View)FloatRect(T::getPosition().x-T::getOutlineThickness(), T::getPosition().y + offset, T::getSize().x + T::getOutlineThickness()*2, offset*5));
        
        int start = floor(start_show), end = ceil(end_show);
        tmp.move({0,(start-start_show)*offset});
        tmp_text.move({0, (start-start_show)*offset});
        for (int i = start; i<m_suggestion_list.size() && i<end; i++) {
            if (i==m_hover_index) tmp.setFillColor(button_setting->hover_color);
            tmp_text.move({tmp_text.getLocalBounds().width/2, 0});
            tmp_text.setString(m_suggestion_list[i]);
            tmp.move({0,offset});

            tmp_text.move({-tmp_text.getLocalBounds().width/2, offset});
            texture.draw(tmp, state);
            texture.draw(tmp_text, state);
            if (i==m_hover_index) tmp.setFillColor(button_setting->normal_color);
        }
        texture.display();
        Sprite sprite(texture.getTexture());
        sprite.setPosition(T::getPosition().x - T::getOutlineThickness(), T::getPosition().y+offset);
        target.draw(sprite, state);
    }
}
