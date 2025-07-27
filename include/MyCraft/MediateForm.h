#ifndef MEDIATE_FORM_H
#define MEDIATE_FORM_H
#include "Button.h"
#include "CreateWorldForm.h"
#include "Font.h"
#include "Form.h"
#include "MapMenu.h"
#include "ProgressWaitingForm.h"
#include "Texture.h"
#include "WaitingWorldCreateForm.h"
#include "WorldsManage.h"
namespace MyCraft {
    class MediateForm: public MyBase::Form {
    public:
        MediateForm(GLFWwindow* window, const int& index);
        MediateForm(const MediateForm&) = delete;
        ~MediateForm();
        MediateForm& operator=(const MediateForm&) const = delete; 
    protected:
    private:
        MyBase::Texture                 __mapTexture;
        MyBase::TextureButton           __returnButton;
        WorldsManage                    __worldsManage;
        MapMenu                         __menu;
        MyBase::Font                    __font;
        MyBase::CreateWorldForm         __createWorldForm;
        WaitingWorldCreateForm          __waitingCreateMap;
        MyBase::RoundedRectangleButton  __createWorldButton, __clearWorldButton, __joinWorldButton, __connectWorldButton;

        bool __mouseClicked(GLFWwindow*) override;
        bool catchEvent(GLFWwindow* window) override;
    };
}
#endif