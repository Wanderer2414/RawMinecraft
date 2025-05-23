#ifndef SUGGESTION_LIST_H
#define SUGGESTION_LIST_H
#include "Global.h"
#include "Textbox.h"
template<typename T>
class Suggestionlist: public Textbox<T> {
public:
    Suggestionlist(ButtonSetting& button_setting = ButtonSetting::Default, TextSetting& text_setting = TextSetting::Default);
    ~Suggestionlist();
    virtual bool    contains(const Vector2f& position) const override,
                    setHover(const Vector2f& position) override;
    size_t          getSelectionValue() const;
    virtual void    setList(const vector<string>& list, const vector<size_t>& values = {}),
                    setAutoComplete(const bool& autocommplete);
    unsigned int    getSelectionIndex() const;
    virtual void    setDirection(const bool& downward);
protected:
    float start_show, end_show, wheel_delta;

    virtual void reset() override;
    virtual catch_function(CatchEvent) override;
    virtual catch_function(AfterCatch) override;
    virtual void draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    bool            m_autocomplete,
                    is_downward;
    vector<string>  m_suggestion_list;
    vector<size_t>  value;
    ButtonSetting*  button_setting;
    unsigned int    m_selection_index;
    int             m_hover_index;
private:
};
template class Suggestionlist<MC::Rectangle>;
template class Suggestionlist<RoundedRectangle>;
#endif