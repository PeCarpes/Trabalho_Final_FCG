
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <textrendering.h>
#include <Game.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering_ShowCameraInfo(GLFWwindow *window, Camera &cam, float x, float y);
void TextRendering_ShowModelViewProjection(GLFWwindow *window, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 p_model);
void TextRendering_ShowProjection(GLFWwindow *window);
void TextRendering_ShowFramesPerSecond(GLFWwindow *window);

// Função auxiliar para criar e adicionar uma parede ao VirtualScene.

const int screen_width = 1200;
const int screen_height = 800;

bool g_ShowInfoText = false;

int main(void)
{

    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    fflush(stdout);

    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "INF01047 - Seu Cartao - Seu Nome", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    fflush(stdout);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    fflush(stdout);

    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *glversion = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* =================== ID COLLECTION =================== */
    // Obs: Se atualizar aqui, também atualizar em shader_fragment.glsl

#define WEAPON 0
#define ENEMY 1
#define CUBE 2
#define WALL 3
#define DEBUG 4
#define PROJECTILE 5 // !! ATUALIZAR ID DENTRO DE Projectile.cpp CASO MUDAR AQUI !!

    Game game = Game();
    game.initializeShader();
    game.initializeSoundEngine();

    Callbacks::initializeCallbacks(window);

    /* =================== SOUNDS =================== */
    game.addSound("walking_sound", "../../data/walking.wav");
    game.addSound("shooting_sound", "../../data/shooting_sound.wav");

    /* =================== TEXTURES =================== */

    game.addTexture("projectile_texture", "../../data/projectile_texture.png");
    game.addTexture("ceiling_texture", "../../data/ceiling_texture.png");
    game.addTexture("floor_texture",  "../../data/floor_texture.png");
    game.addTexture("weapon_texture", "../../data/Pistol_01_Albedo.png");
    game.addTexture("wall_texture",   "../../data/wall_texture.png");

    /* =================== OBJECT MODELS =================== */

    game.addObjModel("weapon_obj",     "../../data/Pistol_01.obj");
    game.addObjModel("projectile_obj", "../../data/projectile.obj");
    game.addObjModel("enemy_obj",      "../../data/enemy.obj");
    game.addObjModel("cube_obj",       "../../data/cube.obj");

    /* =================== SCENE OBJECTS =================== */

    game.addSceneObject("weapon_sobj",     "weapon_obj",     "weapon_texture", WEAPON,     false, false);

    // etapa 1
    game.addEnemy(glm::vec4(4.5f, 1.5f, 0.0f, 1.0f));
    game.addEnemy(glm::vec4(1.5f, 1.5f, 4.5f, 1.0f));
    game.addEnemy(glm::vec4(7.5f, 1.5f, 4.5f, 1.0f));
    game.addEnemy(glm::vec4(3.5f, 1.5f, 8.5f, 1.0f));
    game.addEnemy(glm::vec4(5.5f, 1.5f, 8.5f, 1.0f));

    // etapa 2
    game.addEnemy(glm::vec4(3.0f, 1.5f, 11.0f, 1.0f));
    game.addEnemy(glm::vec4(0.0f, 1.5f, 14.0f, 1.0f));

    // etapa 3
    game.addEnemy(glm::vec4(11.5f, 1.5f, 13.5f, 1.0f));
    game.addEnemy(glm::vec4(11.5f, 1.5f, 13.5f, 1.0f));
    game.addEnemy(glm::vec4(16.0f, 1.5f, 14.0f, 1.0f));
    game.addEnemy(glm::vec4(16.0f, 1.5f, 15.0f, 1.0f));

    /* =================== INITIALIZERS =================== */

    game.initializePlayer("weapon_sobj", "projectile_obj", "projectile_texture");
    game.initializeCamera();

    /* =================== MODIFY OBJECTS =================== */

    game.setObjectScale("weapon_sobj", glm::vec3(0.03f, 0.03f, 0.03f));
    game.setObjectRotation("weapon_sobj", glm::vec3(0.0f, 90.0f, 0.0f));

    /* =================== CREATE LEVEL OBJECTS =================== */

    game.addSceneObject("floor1", "cube_obj", "floor_texture", CUBE, true, true);
    game.setObjectScale("floor1", glm::vec3(5.0f, 1.0f, 5.0f));
    game.setObjectTextureScale("floor1", glm::vec2(50.0f, 50.0f));
    game.setObjectPosition("floor1", glm::vec4(4.5f, 0.0f, 4.5f, 1.0f));
    
    game.addSceneObject("floor2", "cube_obj", "floor_texture", CUBE, true, true);
    game.setObjectScale("floor2", glm::vec3(5.0f, 1.0f, 3.0f));
    game.setObjectTextureScale("floor2", glm::vec2(50.0f, 50.0f));
    game.setObjectPosition("floor2", glm::vec4(4.5f, 0.0f, 12.5f, 1.0f));

    game.addSceneObject("floor3", "cube_obj", "floor_texture", CUBE, true, true);
    game.setObjectScale("floor3", glm::vec3(4.0f, 1.0f, 2.0f));
    game.setObjectTextureScale("floor3", glm::vec2(50.0f, 50.0f));
    game.setObjectPosition("floor3", glm::vec4(13.5f, 0.0f, 14.5f, 1.0f));

    /* ===================================================== */
    /* ======================== WALLS ====================== */
    
    // Create pillars in main hall
    for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                std::string name = "pillar_" + std::to_string(i) + "_" + std::to_string(j);
                game.addSceneObject(name, "cube_obj", "wall_texture", WALL, true, true);
                game.setObjectScale(name, glm::vec3(0.5f, 2.0f, 0.5f));
                game.setObjectTextureScale(name, glm::vec2(5.0f, 5.0f));
                game.setObjectPosition(name, glm::vec4(2 + (5 * j), 1.5f, 2 + (5 * i), 1.0f));
            }
        }
    
    // Create walls around the main hall
    game.addSceneObject("wall_1", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_1", glm::vec3(5.0f, 2.0f, 0.5f));
    game.setObjectTextureScale("wall_1", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_1", glm::vec4(4.5f, 1.5f, -1.0f, 1.0f));

    game.addSceneObject("wall_2", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_2", glm::vec3(0.5f, 2.0f, 8.0f));
    game.setObjectTextureScale("wall_2", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_2", glm::vec4(-1.0f, 1.5f, 7.5f, 1.0f));

    game.addSceneObject("wall_3", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_3", glm::vec3(4.0f, 2.0f, 6.5f));
    game.setObjectTextureScale("wall_3", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_3", glm::vec4(13.5f, 1.5f, 6.0f, 1.0f));

    game.addSceneObject("wall_4", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_4", glm::vec3(2.0f, 2.0f, 0.5f));
    game.setObjectTextureScale("wall_4", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_4", glm::vec4(1.5f, 1.5f, 10.0f, 1.0f));

    game.addSceneObject("wall_5", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_5", glm::vec3(2.0f, 2.0f, 2.0f));
    game.setObjectTextureScale("wall_5", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_5", glm::vec4(7.5f, 1.5f, 11.5f, 1.0f));

    game.addSceneObject("wall_6", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_6", glm::vec3(2.5f, 2.0f, 1.0f));
    game.setObjectTextureScale("wall_6", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_6", glm::vec4(3.0f, 1.5f, 12.5f, 1.0f));

    game.addSceneObject("wall_7", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_7", glm::vec3(5.0f, 2.0f, 0.5f));
    game.setObjectTextureScale("wall_7", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_7", glm::vec4(4.5f, 1.5f, 16.0f, 1.0f));
    
    game.addSceneObject("wall_8", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_8", glm::vec3(4.0f, 2.0f, 0.5f));
    game.setObjectTextureScale("wall_8", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_8", glm::vec4(13.5f, 1.5f, 17.0f, 1.0f));

    game.addSceneObject("wall_9", "cube_obj", "wall_texture", WALL, true, true);
    game.setObjectScale("wall_9", glm::vec3(0.5f, 2.0f, 2.0f));
    game.setObjectTextureScale("wall_9", glm::vec2(5.0f, 5.0f));
    game.setObjectPosition("wall_9", glm::vec4(18.0f, 1.5f, 14.5f, 1.0f));

    game.addSceneObject("ceiling", "cube_obj", "ceiling_texture", WALL, true, true);
    game.setObjectScale("ceiling", glm::vec3(10.0f, 0.1f, 9.5f));
    game.setObjectTextureScale("ceiling", glm::vec2(2.0f, 2.0f));
    game.setObjectPosition("ceiling", glm::vec4(8.5f, 3.0f, 8.0f, 1.0f));

    TextRendering_Init();

    GameState currentGameState = GameState::IN_MENU;

    glfwSetWindowUserPointer(window, &game);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Callbacks::updateDeltaTime();

        game.updateCurrentGameState(&currentGameState);
        game.updateCameraMode();

        switch (currentGameState)
        {
            case GameState::IN_MENU:
            {
                TextRendering_PrintString(window, "SUFFICIENTLY WARM", -0.5f, 0.5f, 1.5f);
                TextRendering_PrintString(window, "Aperte [Enter] para comecar", -0.7f, 0.0f, 1.0f);
                TextRendering_PrintString(window, "Aperte [Esc] para sair", -0.6f, -0.2f, 1.0f);
                break;
            }
        
            case GameState::IN_GAME:
            {    
                game.updateCamera();
                game.useShader();

                if (game.getCameraMode() == CameraMode::FOLLOW_PROJECTILE)
                {
                    // esconde a arma do jogador
                    game.setObjectPosition("weapon_sobj", glm::vec4(100.0f, 100.0f, 100.0f, 1.0f));
                }
                else
                { 
                    game.allowPlayerToFly();
                    game.movePlayer();
                }

                game.moveEnemies();
                game.moveProjectiles();

                game.manageEnemyShooting();
                game.managePlayerShooting();

                game.deleteMarkedObjects();

                game.draw();

                glm::vec4 p_model(0.0f, 0.0f, 0.0f, 1.0f);
                TextRendering_ShowModelViewProjection(window, game.getCamera()->getProjectionMatrix(), game.getCamera()->getViewMatrix(), Matrix_Identity(), p_model);
                TextRendering_ShowCameraInfo(window, *game.getCamera(), -1.0f, -0.5f);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void TextRendering_ShowCameraInfo(GLFWwindow *window, Camera &cam, float x, float y)
{
    float lineheight = TextRendering_LineHeight(window);
    char buffer[100]; // Buffer para formatar as strings

    TextRendering_PrintString(window, "--- Camera Debug Info ---", x, y, 0.8f);

    // Imprime a Posição da Câmera
    snprintf(buffer, 100, "Pos: (%.2f, %.2f, %.2f, %.1f)", cam.position.x, cam.position.y, cam.position.z, cam.position.w);
    TextRendering_PrintString(window, buffer, x, y - lineheight, 0.8f);

    // Imprime o Vetor "Para Frente" (Direção)
    snprintf(buffer, 100, "Fwd: (%.2f, %.2f, %.2f, %.1f)", cam.forward_vector.x, cam.forward_vector.y, cam.forward_vector.z, cam.forward_vector.w);
    TextRendering_PrintString(window, buffer, x, y - 2 * lineheight, 0.8f);

    // Imprime o Vetor "Para Cima"
    snprintf(buffer, 100, "Up:  (%.2f, %.2f, %.2f, %.1f)", cam.up_vector.x, cam.up_vector.y, cam.up_vector.z, cam.up_vector.w);
    TextRendering_PrintString(window, buffer, x, y - 3 * lineheight, 0.8f);

    // Imprime os Ângulos de Euler
    snprintf(buffer, 100, "Yaw: %.2f, Pitch: %.2f", cam.yaw, cam.pitch);
    TextRendering_PrintString(window, buffer, x, y - 4 * lineheight, 0.8f);

    snprintf(buffer, 100, "FPS: %.2f", 1.0f / Callbacks::getDeltaTime());
    TextRendering_PrintString(window, buffer, x, y - 5 * lineheight, 0.8f);
}

// Esta função recebe um vértice com coordenadas de modelo p_model e passa o
// mesmo por todos os sistemas de coordenadas armazenados nas matrizes model,
// view, e projection; e escreve na tela as matrizes e pontos resultantes
// dessas transformações.
void TextRendering_ShowModelViewProjection(
    GLFWwindow *window,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::vec4 p_model)
{
    if (!g_ShowInfoText)
        return;

    glm::vec4 p_world = model * p_model;
    glm::vec4 p_camera = view * p_world;
    glm::vec4 p_clip = projection * p_camera;
    glm::vec4 p_ndc = p_clip / p_clip.w;

    float pad = TextRendering_LineHeight(window);

    TextRendering_PrintString(window, " Model matrix             Model     In World Coords.", -1.0f, 1.0f - pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, model, p_model, -1.0f, 1.0f - 2 * pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f - 6 * pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f - 7 * pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f - 8 * pad, 1.0f);

    TextRendering_PrintString(window, " View matrix              World     In Camera Coords.", -1.0f, 1.0f - 9 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, view, p_world, -1.0f, 1.0f - 10 * pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f - 14 * pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f - 15 * pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f - 16 * pad, 1.0f);

    TextRendering_PrintString(window, " Projection matrix        Camera                    In NDC", -1.0f, 1.0f - 17 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProductDivW(window, projection, p_camera, -1.0f, 1.0f - 18 * pad, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glm::vec2 a = glm::vec2(-1, -1);
    glm::vec2 b = glm::vec2(+1, +1);
    glm::vec2 p = glm::vec2(0, 0);
    glm::vec2 q = glm::vec2(width, height);

    glm::mat4 viewport_mapping = Matrix(
        (q.x - p.x) / (b.x - a.x), 0.0f, 0.0f, (b.x * p.x - a.x * q.x) / (b.x - a.x),
        0.0f, (q.y - p.y) / (b.y - a.y), 0.0f, (b.y * p.y - a.y * q.y) / (b.y - a.y),
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    TextRendering_PrintString(window, "                                                       |  ", -1.0f, 1.0f - 22 * pad, 1.0f);
    TextRendering_PrintString(window, "                            .--------------------------'  ", -1.0f, 1.0f - 23 * pad, 1.0f);
    TextRendering_PrintString(window, "                            V                           ", -1.0f, 1.0f - 24 * pad, 1.0f);

    TextRendering_PrintString(window, " Viewport matrix           NDC      In Pixel Coords.", -1.0f, 1.0f - 25 * pad, 1.0f);
    TextRendering_PrintMatrixVectorProductMoreDigits(window, viewport_mapping, p_ndc, -1.0f, 1.0f - 26 * pad, 1.0f);
}

// Escrevemos na tela qual matriz de projeção está sendo utilizada.
void TextRendering_ShowProjection(GLFWwindow *window)
{
    if (!g_ShowInfoText)
        return;
}

// Escrevemos na tela o número de quadros renderizados por segundo (frames per
// second).
void TextRendering_ShowFramesPerSecond(GLFWwindow *window)
{
    if (!g_ShowInfoText)
        return;

    // Variáveis estáticas (static) mantém seus valores entre chamadas
    // subsequentes da função!
    static float old_seconds = (float)glfwGetTime();
    static int ellapsed_frames = 0;
    static char buffer[20] = "?? fps";
    static int numchars = 7;

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;

    if (ellapsed_seconds > 1.0f)
    {
        numchars = snprintf(buffer, 20, "%.2f fps", ellapsed_frames / ellapsed_seconds);

        old_seconds = seconds;
        ellapsed_frames = 0;
    }

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, buffer, 1.0f - (numchars + 1) * charwidth, 1.0f - lineheight, 1.0f);
}

