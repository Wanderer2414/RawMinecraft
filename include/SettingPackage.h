#ifndef SETTINGPACKAGE_H
#define SETTINGPACKAGE_H

#include "Global.h"

class ButtonSetting {
public:
static  ButtonSetting Default;
Color   click_color ,     
        hover_color ,
        normal_color;
};
class TextSetting {
public:
static  TextSetting Default;
Font    font;
float   font_size,
        medium_font_size,
        small_font_size,
        spacing;
Color   font_color;
};


class FormSetting {
public:

static  FormSetting Default;
Color   middle_color;
Color   background_color,
        reverse_color,
        middle_reverse_color;
Texture         *check_button_normal,
                *check_button_hover,
                *check_button_click,
                *no_button_normal,
                *no_button_hover,
                *no_button_click;
};
class Setting {
public:
static const    Setting Default;
FormSetting     *form_setting;
ButtonSetting   *button_setting;
TextSetting     *test_setting;
};

#endif