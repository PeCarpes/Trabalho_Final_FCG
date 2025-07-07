#include <Callbacks.h>
#include <Game.h>
#include <iostream>

void Callbacks::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {

        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        // Toggle camera mode
        if (game->getCameraMode() == CameraMode::FIRST_PERSON)
        {
            game->setBoolAskCamera(true);
        }
        else if (game->getCameraMode() == CameraMode::FOLLOW_PROJECTILE)
        {
            game->setBoolAskCamera(false);
        }
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        // Reset the level
        game->resetLevel();
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        // Start the game
        if (game->getGameState() == GameState::IN_MENU)
        {
            game->setGameState(GameState::IN_GAME);
        }
    }

    key_states[key] = action;
}
void Callbacks::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    cursor_pos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

void Callbacks::updateTimeModifier(double modifier)
{
    time_modifier = modifier;
}

double Callbacks::getTimeModifier()
{
    return time_modifier;
}

void Callbacks::updateDeltaTime()
{
    current_time = glfwGetTime();
    delta_time = current_time - last_time;
    last_time = current_time;
}

double Callbacks::getDeltaTime()
{
    return delta_time;
}

glm::vec2 Callbacks::getMouseOffset()
{
    glm::vec2 offset = cursor_pos - last_cursor_pos;
    last_cursor_pos = cursor_pos; // Update last position
    return offset;
}

void Callbacks::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        left_mouse_button_pressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        left_mouse_button_pressed = false;
    }
}

void Callbacks::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{   
    glViewport(0, 0, width, height);

    screen_ratio = (float)width / height;
}

void Callbacks::initializeCallbacks(GLFWwindow *window)
{
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize the cursor position to the center of the window
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    cursor_pos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    last_cursor_pos = cursor_pos;
}

glm::vec2 Callbacks::getCursorPosition()
{
    return cursor_pos;
}

bool Callbacks::isLeftMouseButtonPressed()
{
    return left_mouse_button_pressed;
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

float Callbacks::getScreenRatio()
{
    return screen_ratio;
}