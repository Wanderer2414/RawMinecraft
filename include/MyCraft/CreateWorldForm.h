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
        bool isSubmit() const;
        void clear();
        std::string getName() const;
        Date getDate() const;
    protected:
    private:
        bool                    __isSubmit;
        Date                    __date;
        RoundedRectangleButton  __createButton;
        Label                   __nameLabel, __dateLabel;
        Textbox                 __nameInput;

        void __open(GLFWwindow* ) override;
        bool __mouseClicked(GLFWwindow*) override;
    };
}
#endif