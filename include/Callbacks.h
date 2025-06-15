#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>

class Callbacks
{
private:
    static inline glm::vec2 cursor_pos = glm::vec2(0.0f, 0.0f);

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static std::unordered_map<int, int> key_states;

public:
    static void initializeCallbacks(GLFWwindow *window);
    static glm::vec2 getCursorPosition();

    static int getKeyState(int key);
};