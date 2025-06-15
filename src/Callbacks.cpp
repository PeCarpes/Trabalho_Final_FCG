#include <Callbacks.h>

void Callbacks::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {

        glfwSetWindowShouldClose(window, true);
    }

    key_states[key] = action;
}
void Callbacks::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    cursor_pos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

void Callbacks::initializeCallbacks(GLFWwindow *window)
{
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}

glm::vec2 Callbacks::getCursorPosition()
{
    return cursor_pos;
}

int Callbacks::getKeyState(int key)
{
    auto it = key_states.find(key);
    if (it != key_states.end())
    {
        return it->second;
    }
    key_states[key] = GLFW_RELEASE; // Default to not pressed if not found
    return GLFW_RELEASE;
}