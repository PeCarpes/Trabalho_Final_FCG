#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>



typedef std::unordered_map<int, int> KeyStateMap;

class Callbacks
{
private:

    static inline double current_time = 0.0f;
    static inline double last_time = 0.0f;
    static inline double delta_time = 0.0f;

    static inline glm::vec2 last_cursor_pos = glm::vec2(0.0f, 0.0f);
    static inline glm::vec2 cursor_pos = glm::vec2(0.0f, 0.0f);
    static inline bool left_mouse_button_pressed = false;
    static inline float screen_ratio = 1.0f;

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static inline KeyStateMap key_states;

public:
    static void initializeCallbacks(GLFWwindow *window);
    static glm::vec2 getCursorPosition();
    static bool isLeftMouseButtonPressed();
    static float getScreenRatio();
    static glm::vec2 getMouseOffset();

    static void updateDeltaTime();
    static double getDeltaTime();

    static int getKeyState(int key);
};