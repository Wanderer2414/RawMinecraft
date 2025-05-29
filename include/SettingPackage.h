#ifndef SETTINGPACKAGE_H
#define SETTINGPACKAGE_H

#include "Global.h"

class ButtonSetting {
public:
static  ButtonSetting Default;
sf::Color       click_color ,     
                hover_color ,
                normal_color;
};
class TextSetting {
public:
static  TextSetting Default;
sf::Font        font;
float           font_size,
                medium_font_size,
                small_font_size,
                spacing;
sf::Color       font_color;
};


class FormSetting {
public:

static  FormSetting Default;
sf::Color       middle_color;
sf::Color      background_color,
                reverse_color,
                middle_reverse_color;
sf::Texture     *check_button_normal,
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