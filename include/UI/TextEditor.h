#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H
#include "Rectangle.h"
#include "SettingPackage.h"
#include "Textbox.h"

class TextEditor: public Textbox<Rectangle> {
public:
    TextEditor(const ButtonSetting& button_setting = ButtonSetting::Default, const TextSetting& text_setting = TextSetting::Default);
    ~TextEditor();
    uint            getSelectionValue() const;
    virtual void    setList(const vector<string>& list, const vector<size_t>& values),
                    setString(const string& text) override,
                    update() override;
    string          getPreviousString() const;

protected:
    uint            shared_selection_index;
    virtual void draw(RenderTarget &target, RenderStates state) const override;

    vector<string> shared_list;
    vector<size_t> shared_values;
};
#endif