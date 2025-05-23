#include "SettingPackage.h"

ButtonSetting ButtonSetting::Default = {
    {54, 63, 68, 255},    // Button Pressed (Deepest Green-Blue)
    {24, 61, 61, 255},   // Button Hover (Muted Dark Teal)
    {92, 131, 116, 255},  // Button (Soft Desaturated Green)
};
    
TextSetting TextSetting::Default = {
    {}, 30, 25, 15, 2,
    {220, 230, 225, 255}, // Text (Light Grayish Green for readability)
};
FormSetting FormSetting::Default = {
    {147, 177, 166, 255},    // Border or other UI elements (Muted Greenish-Gray)
    {4, 13, 18, 255},         // Background (Deep Dark Green-Blue)
    {240, 240, 240, 255},
    {100, 100, 100, 255}
};