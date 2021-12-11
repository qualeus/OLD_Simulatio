#include "../../include/Context/Inputs.hpp"

namespace ctx {
Inputs::Inputs() {}

bool Inputs::pressed_mouse[MOUSE_INPUT_SIZE] = {false};
bool Inputs::current_mouse[MOUSE_INPUT_SIZE] = {false};
bool Inputs::released_mouse[MOUSE_INPUT_SIZE] = {false};

bool Inputs::pressed_keys[KEYS_INPUT_SIZE] = {false};
bool Inputs::current_keys[KEYS_INPUT_SIZE] = {false};
bool Inputs::released_keys[KEYS_INPUT_SIZE] = {false};

void Inputs::ResetInputs() {
    Inputs::ResetKeyboardInputs();
    Inputs::ResetMouseInputs();
}

void Inputs::ResetKeyboardInputs() {
    for (int i = 0; i < KEYS_INPUT_SIZE; i++) { pressed_keys[i] = false; }
    for (int i = 0; i < KEYS_INPUT_SIZE; i++) { released_keys[i] = false; }
}

void Inputs::ResetMouseInputs() {
    for (int i = 0; i < MOUSE_INPUT_SIZE; i++) { pressed_mouse[i] = false; }
    for (int i = 0; i < MOUSE_INPUT_SIZE; i++) { released_mouse[i] = false; }
}

void Inputs::HandleKeyboardInputs(int key, int action) {
    if (key >= 0 && key < KEYS_INPUT_SIZE) {
        if (action == 1) {  // GLFW_PRESS
            current_keys[key] = true;
            pressed_keys[key] = true;
        } else if (action == 0) {  // GLFW_RELEASE
            current_keys[key] = false;
            released_keys[key] = true;
        }
    }
}

void Inputs::HandleMouseInputs(int button, int action) {
    if (button >= 0 && button < MOUSE_INPUT_SIZE) {
        if (action == 1) {  // GLFW_PRESS
            current_mouse[button] = true;
            pressed_mouse[button] = true;
        } else if (action == 0) {  // GLFW_RELEASE
            current_mouse[button] = false;
            released_mouse[button] = true;
        }
    }
}

bool Inputs::KeyPressed(int key) { return pressed_keys[key]; }
bool Inputs::KeyDown(int key) { return current_keys[key]; }
bool Inputs::KeyReleased(int key) { return released_keys[key]; }

bool Inputs::MousePressed(int key) { return pressed_mouse[key]; }
bool Inputs::MouseDown(int key) { return current_mouse[key]; }
bool Inputs::MouseReleased(int key) { return released_mouse[key]; }

}  // namespace ctx