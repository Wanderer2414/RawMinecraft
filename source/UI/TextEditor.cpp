#include "TextEditor.h"
#include "Rectangle.h"
#include "SFML/Graphics/Sprite.hpp"

namespace MyBase {

    TextEditor::TextEditor(const ButtonSetting& button_setting, const TextSetting& text_setting) {
        _isMultiLine = true;
    }
    TextEditor::~TextEditor() {
    }
    uint TextEditor::getSelectionValue() const {
        if (shared_selection_index>=0 && shared_selection_index<_values.size())
            return _values[shared_selection_index];
        return 0;
    }
    std::string TextEditor::getPreviousString() const {
        return _content.substr(0, _cursorIndex);
    }
    void TextEditor::setList(const std::vector<std::string>& list, const std::vector<std::size_t>& values) {
        _list = list;
        _values = values;
        shared_selection_index = list.size();
    }
    void TextEditor::update() {
        Rectangle::update();
        Text::setPosition(0, 0);
    }
    void TextEditor::setString(const std::string& text) {
        Textbox::setString(text);
        _list.clear();
        _values.clear();
    }
    void TextEditor::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        Textbox::draw(target, state);
        if (_list.size()) {
            Text text = *this;
            sf::RenderTexture texture;
            float height = text.getCharacterSize() + 5;
            Rectangle rec;
            rec.setFillColor(_buttonSetting->normal_color);
            rec.setSize(300, height);
            texture.create(300, std::min(size_t(5), _list.size())*height);
            texture.clear(_buttonSetting->click_color);
            text.setPosition(0, 0);
            for (auto& i:_list) {
                text.setString(i);
                texture.draw(rec, state);
                texture.draw(text, state);
                rec.move({0, height});
                text.move({0, height});
            }
            texture.display();
            sf::Sprite sprite(texture.getTexture());
            sprite.setPosition(_cursor.getPosition().x + getPosition().x, getPosition().y + _cursor.getSize().y);
            target.draw(sprite, state);
        }
    }
    
}