#ifndef SUGGESTION_LIST_H
#define SUGGESTION_LIST_H
#include "Global.h"
#include "Textbox.h"
namespace MyBase {

    template<typename T>
    class Suggestionlist: public Textbox<T> {
    public:
        Suggestionlist(ButtonSetting& button_setting = ButtonSetting::Default, TextSetting& text_setting = TextSetting::Default);
        ~Suggestionlist();
        virtual bool    contains(const sf::Vector2f& position) const override,
                        setHover(const sf::Vector2f& position) override;
        size_t          getSelectionValue() const;
        virtual void    setList(const std::vector<std::string>& list, const std::vector<size_t>& values = {}),
                        setAutoComplete(const bool& autocommplete);
        unsigned int    getSelectionIndex() const;
        virtual void    setDirection(const bool& downward);
    protected:
        float _startShow, _endShow, _wheelDelta;
    
        virtual void reset() override;
        virtual catch_function(CatchEvent) override;
        virtual catch_function(AfterCatch) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
        bool            __autoComplete,
                        __isDownward;
        std::vector<std::string>  __suggestionList;
        std::vector<std::size_t>  __value;
        ButtonSetting*  __buttonSetting;
        unsigned int    __selectionIndex;
        int             __hoverIndex;
    private:
    };
    template class Suggestionlist<Rectangle>;
    template class Suggestionlist<RoundedRectangle>;
}
#endif