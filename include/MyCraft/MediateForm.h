#ifndef MEDIATE_FORM_H
#define MEDIATE_FORM_H
#include "Button.h"
#include "CreateWorldForm.h"
#include "Font.h"
#include "Form.h"
#include "MapMenu.h"
namespace MyCraft {
    class MediateForm: public MyBase::Form {
    public:
        MediateForm(GLFWwindow* window, const int& index);
        MediateForm(const MediateForm&) = delete;
        ~MediateForm();
        MediateForm& operator=(const MediateForm&) const = delete; 
    protected:
    private:
        MapMenu                         __menu;
        MyBase::Font                    __font;
        MyBase::CreateWorldForm         __createWorldForm;
        MyBase::RoundedRectangleButton  __createWorldButton, __clearWorldButton, __joinWorldButton, __connectWorldButton;

        bool catchEvent(GLFWwindow* window) override;
    };
}
#endif