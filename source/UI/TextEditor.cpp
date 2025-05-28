#include "TextEditor.h"
#include "BaseShape.h"
#include "Rectangle.h"
#include "SFML/Graphics/Sprite.hpp"
namespace MyBase {

    TextEditor::TextEditor(const ButtonSetting& button_setting, const TextSetting& text_setting) {
        m_is_multiline = true;
    }
    TextEditor::~TextEditor() {
    }
    uint TextEditor::getSelectionValue() const {
        if (shared_selection_index>=0 && shared_selection_index<shared_values.size())
            return shared_values[shared_selection_index];
        return 0;
    }
    string TextEditor::getPreviousString() const {
        return m_content.substr(0, m_cursor_index);
    }
    void TextEditor::setList(const vector<string>& list, const vector<std::size_t>& values) {
        shared_list = list;
        shared_values = values;
        shared_selection_index = list.size();
    }
    void TextEditor::update() {
        Rectangle::update();
        Text::setPosition(0, 0);
    }
    void TextEditor::setString(const string& text) {
        Textbox::setString(text);
        shared_list.clear();
        shared_values.clear();
    }
    void TextEditor::draw(RenderTarget& target, RenderStates state) const {
        Textbox::draw(target, state);
        if (shared_list.size()) {
            Text text = *this;
            RenderTexture texture;
            float height = text.getCharacterSize() + 5;
            Rectangle rec;
            rec.setFillColor(button_setting->normal_color);
            rec.setSize(300, height);
            texture.create(300, min(size_t(5), shared_list.size())*height);
            texture.clear(button_setting->click_color);
            text.setPosition(0, 0);
            for (auto& i:shared_list) {
                text.setString(i);
                texture.draw(rec, state);
                texture.draw(text, state);
                rec.move({0, height});
                text.move({0, height});
            }
            texture.display();
            Sprite sprite(texture.getTexture());
            sprite.setPosition(cursor.getPosition().x + getPosition().x, getPosition().y + cursor.getSize().y);
            target.draw(sprite, state);
        }
    }
    
}