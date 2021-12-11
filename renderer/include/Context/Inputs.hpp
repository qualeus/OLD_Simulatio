#ifndef Inputs_HPP
#define Inputs_HPP

namespace ctx {

#define MOUSE_INPUT_SIZE 8
#define KEYS_INPUT_SIZE 512

class Inputs {
   private:
    Inputs();

   public:
    static bool pressed_mouse[MOUSE_INPUT_SIZE];
    static bool current_mouse[MOUSE_INPUT_SIZE];
    static bool released_mouse[MOUSE_INPUT_SIZE];

    static bool pressed_keys[KEYS_INPUT_SIZE];
    static bool current_keys[KEYS_INPUT_SIZE];
    static bool released_keys[KEYS_INPUT_SIZE];

    static void ResetInputs();
    static void ResetKeyboardInputs();
    static void ResetMouseInputs();

    static void HandleKeyboardInputs(int key, int action);
    static void HandleMouseInputs(int button, int action);

    static bool KeyPressed(int key);
    static bool KeyDown(int key);
    static bool KeyReleased(int key);

    static bool MousePressed(int key);
    static bool MouseDown(int key);
    static bool MouseReleased(int key);
};

}  // namespace ctx
#endif