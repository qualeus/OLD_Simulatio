#ifndef GuiTheme_HPP
#define GuiThemee_HPP

#include "imgui.h"
#include "imgui_internal.h"

namespace ovl {

class GuiTheme {
   private:
    GuiTheme();

    // 0xRRGGBBAA
    inline static int BackGroundColor = 0x25213100;
    inline static int TextColor = 0xF4F1DE00;
    inline static int MainColor = 0xDA115E00;
    inline static int MainAccentColor = 0x79235900;
    inline static int HighlightColor = 0xC7EF0000;

    inline static int Black = 0x00000000;
    inline static int White = 0xFFFFFF00;

    inline static int AlphaTransparent = 0x00;
    inline static int Alpha20 = 0x33;
    inline static int Alpha40 = 0x66;
    inline static int Alpha50 = 0x80;
    inline static int Alpha60 = 0x99;
    inline static int Alpha80 = 0xCC;
    inline static int Alpha90 = 0xE6;
    inline static int AlphaFull = 0xFF;

    static float GetR(int colorCode);
    static float GetG(int colorCode);
    static float GetB(int colorCode);
    static float GetA(int alphaCode);

    static ImVec4 GetColor(int c, int a = Alpha80);
    static ImVec4 Darken(ImVec4 c, float p);
    static ImVec4 Lighten(ImVec4 c, float p);

    static ImVec4 Disabled(ImVec4 c);
    static ImVec4 Hovered(ImVec4 c);
    static ImVec4 Active(ImVec4 c);
    static ImVec4 Collapsed(ImVec4 c);

   public:
    static void SetColors(int backGroundColor, int textColor, int mainColor, int mainAccentColor, int highlightColor);
    static void ApplyTheme();
};

}  // namespace ovl
#endif