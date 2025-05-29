#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H
#include "Rectangle.h"
#include "SettingPackage.h"
#include "Textbox.h"
#include "Global.h"
namespace MyBase {

class TextEditor: public Textbox<Rectangle> {
    public:
        TextEditor(const ButtonSetting& button_setting = ButtonSetting::Default, const TextSetting& text_setting = TextSetting::Default);
        ~TextEditor();
        uint            getSelectionValue() const;
        virtual void    setList(const std::vector<std::string>& list, const std::vector<std::size_t>& values),
                        setString(const std::string& text)  override,
                        update()                            override;
        std::string     getPreviousString() const;
    
    protected:
        uint            shared_selection_index;
        virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
    
        std::vector<std::string> _list;
        std::vector<std::size_t> _values;
    };
}
#endif