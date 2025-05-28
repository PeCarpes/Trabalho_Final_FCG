#include <Callbacks.h>

void Callbacks::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        
        glfwSetWindowShouldClose(window, true);
    }
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