#ifndef CREATE_WORLD_FORM_H
#define CREATE_WORLD_FORM_H
#include "Button.h"
#include "Date.h"
#include "Label.h"
#include "MessageBox.h"
#include "Textbox.h"
namespace MyBase {
    class CreateWorldForm: public MessageBox {
    public:
        CreateWorldForm(const Font& font);
        CreateWorldForm(const CreateWorldForm&) = delete;
        ~CreateWorldForm();
        CreateWorldForm& operator=(const CreateWorldForm&) const = delete; 
    protected:
    private:
        Date                    __date;
        RoundedRectangleButton  __createButton;
        Label                   __nameLabel, __dateLabel;
        Textbox                 __nameInput;
    };
}
#endif