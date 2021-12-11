#ifndef Inputs_HPP
#define Inputs_HPP

#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>

namespace ctx {

#define MOUSE_INPUT_SIZE 8
#define KEYS_INPUT_SIZE 512

class Inputs {
   private:
    Inputs();

   public:
    enum KEYBOARD { QWERTY, AZERTY };
    static KEYBOARD keyboard;

    static bool pressed_mouse[MOUSE_INPUT_SIZE];
    static bool current_mouse[MOUSE_INPUT_SIZE];
    static bool released_mouse[MOUSE_INPUT_SIZE];

    static bool pressed_keys[KEYS_INPUT_SIZE];
    static bool current_keys[KEYS_INPUT_SIZE];
    static bool released_keys[KEYS_INPUT_SIZE];

    static void ResetInputs();
    static void ResetKeyboardInputs();
    static void ResetMouseInputs();

    static void AssignKeyboard(KEYBOARD i_keyboard);

    static int ConvertKey(int key);
    static int ConvertKeyQuerty(int key);
    static int ConvertKeyAzerty(int key);

    static void HandleKeyboardInputs(int key, int action);
    static void HandleMouseInputs(int button, int action);

    static bool KeyPressed(int key);
    static bool KeyDown(int key);
    static bool KeyReleased(int key);

    static bool MousePressed(int key);
    static bool MouseDown(int key);
    static bool MouseReleased(int key);

    static bool KeyPressed(std::string key);
    static bool KeyDown(std::string key);
    static bool KeyReleased(std::string key);

    static bool MousePressed(std::string key);
    static bool MouseDown(std::string key);
    static bool MouseReleased(std::string key);

    static std::unordered_map<std::string, std::string> QWERTY_TO_AZERTY;
};

}  // namespace ctx
#endif