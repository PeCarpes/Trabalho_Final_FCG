
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ObjModel.h>
#include <VirtualScene.h>
#include <SceneObject.h>
#include <Matrices.h>
#include <Callbacks.h>
#include <Camera.h>
#include <textrendering.h>
#include <Player.h>
#include <Bezier.h>
#include <Enemy.h>
#include <Shader.h>
#include <Texture.h>
#include <Projectile.h>

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
SceneObject* CreateObject(const ObjModel& cube_obj,
                        const std::string& name,
                        Shader& shader,
                        Camera& cam,
                        Texture3D& wall_texture,
                        const glm::vec3& scale,
                        const glm::vec2& texture_scale,
                        int id,
                        const glm::vec3& position,
                        VirtualScene& scene);

const int screen_width = 1200;
const int screen_height = 800;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
GLuint g_GpuProgramID = 0;
GLint g_model_uniform;
GLint g_view_uniform;
GLint g_projection_uniform;
GLint g_object_id_uniform;

VirtualScene g_VirtualScene;
Player g_Player(nullptr, glm::vec4(4.5f, 2.0f, 4.5f, 1.0f)); // Player object

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

    Callbacks::initializeCallbacks(window);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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

    Shader shader = Shader();
    shader.Use();
    Camera cam = Camera(g_Player.getPositionPtr());

    bool show_mouse_cursor = false;

    /* =================== ID COLLECTION =================== */
    // Obs: Se atualizar aqui, também atualizar em shader_fragment.glsl

#define WEAPON 0
#define BUNNY 1
#define ENEMY 2
#define CUBE 3
#define WALL 4

    Texture3D floor_texture;
    floor_texture.LoadTextureImage("../../data/floor_texture.png");
    Texture3D weapon_texture;
    weapon_texture.LoadTextureImage("../../data/Pistol_01_Albedo.png");
    Texture3D wall_texture;
    wall_texture.LoadTextureImage("../../data/wall_texture.png");
    Texture3D ceiling_texure;
    ceiling_texure.LoadTextureImage("../../data/ceiling_texture.png");

    /* =================== WEAPON OBJECT =================== */
    ObjModel weapon_obj("../../data/Pistol_01.obj");
    weapon_obj.ComputeNormals();
    weapon_obj.BuildTriangles();

    SceneObject weapon_sobj(weapon_obj, "weapon", shader, cam, false);
    weapon_sobj.setTexture(weapon_texture);
    weapon_sobj.setID(WEAPON);

    g_VirtualScene.addObject(&weapon_sobj);
    /* ===================================================== */
    /* =================== PROJECTILE OBJECT =================== */
    ObjModel projectile_obj("../../data/projectile.obj");
    projectile_obj.ComputeNormals();
    projectile_obj.BuildTriangles();

    /* ===================================================== */
    /* =================== ENEMY OBJECT =================== */
    ObjModel enemy_obj("../../data/enemy.obj");
    enemy_obj.ComputeNormals();
    enemy_obj.BuildTriangles();

    Enemy enemy(enemy_obj, "enemy1", shader, cam, glm::vec3(4.5f, 3.0f, 0.0f), 1.0f);
    enemy.setID(ENEMY);
    g_VirtualScene.addObject(&enemy);
    enemy.setHeight(1.0f);
    enemy.setProjectileModel(&projectile_obj);

    /* ===================================================== */
    /* =================== CUBE OBJECT ===================== */
    ObjModel cube_obj("../../data/cube.obj");
    cube_obj.ComputeNormals();
    cube_obj.BuildTriangles();

    /* ===================================================== */
    /* ======================= PLAYER ====================== */
    g_Player.initializeWeapon(&weapon_sobj);
    g_Player.initializeProjectiles(&projectile_obj);

    /* ======================= GROUND ====================== */

    CreateObject(cube_obj, "floor1", shader, cam, floor_texture, 
                    glm::vec3(5.0f, 1.0f, 5.0f), glm::vec2(50.0f, 50.0f), CUBE, 
                    glm::vec3(4.5f, 0.0f, 4.5f), g_VirtualScene); 

    CreateObject(cube_obj, "floor2", shader, cam, floor_texture, 
                    glm::vec3(5.0f, 1.0f, 3.0f), glm::vec2(50.0f, 50.0f), CUBE, 
                    glm::vec3(4.5f, 0.0f, 12.5f), g_VirtualScene);

    CreateObject(cube_obj, "floor3", shader, cam, floor_texture, 
                    glm::vec3(4.0f, 1.0f, 2.0f), glm::vec2(50.0f, 50.0f), CUBE, 
                    glm::vec3(13.5f, 0.0f, 14.5f), g_VirtualScene);

    /* ===================================================== */
    /* ======================== WALLS ====================== */
    
    // Create pillars in main hall
    for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                CreateObject(cube_obj, "pillar_" + std::to_string(i) + "_" + std::to_string(j),
                    shader, cam, wall_texture, 
                    glm::vec3(0.5f, 2.0f, 0.5f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(2 + (5 * j), 1.5f, 2 + (5 * i)), g_VirtualScene);
            }
        }


    
    // Create walls around the main hall
    
    CreateObject(cube_obj, "wall_1", shader, cam, wall_texture, 
                    glm::vec3(5.0f, 2.0f, 0.5f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(4.5f, 1.5f, -1.0f), g_VirtualScene);   

    CreateObject(cube_obj, "wall_2", shader, cam, wall_texture, 
                    glm::vec3(0.5f, 2.0f, 8.0f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(-1.0f, 1.5f, 7.5f), g_VirtualScene);

    CreateObject(cube_obj, "wall_3", shader, cam, wall_texture, 
                    glm::vec3(4.0f, 2.0f, 6.5f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(13.5f, 1.5f, 6.0f), g_VirtualScene);

    CreateObject(cube_obj, "wall_4", shader, cam, wall_texture,
                    glm::vec3(2.0f, 2.0f, 0.5f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(1.5f, 1.5f, 10.0f), g_VirtualScene);

    CreateObject(cube_obj, "wall_5", shader, cam, wall_texture,
                    glm::vec3(2.0f, 2.0f, 2.0f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(7.5f, 1.5f, 11.5f), g_VirtualScene);
    
    CreateObject(cube_obj, "wall_6", shader, cam, wall_texture,
                    glm::vec3(2.5f, 2.0f, 1.0f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(3.0f, 1.5f, 12.5f), g_VirtualScene);

    CreateObject(cube_obj, "wall_7", shader, cam, wall_texture, 
                    glm::vec3(5.0f, 2.0f, 0.5f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(4.5f, 1.5f, 16.0f), g_VirtualScene);  
    
    CreateObject(cube_obj, "wall_8", shader, cam, wall_texture, 
                    glm::vec3(4.0f, 2.0f, 0.5f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(13.5f, 1.5f, 17.0f), g_VirtualScene);

    CreateObject(cube_obj, "wall_9", shader, cam, wall_texture, 
                    glm::vec3(0.5f, 2.0f, 2.0f), glm::vec2(5.0f, 5.0f), WALL, 
                    glm::vec3(18.0f, 1.5f, 14.5f), g_VirtualScene); 

    /* ======================================================= */

    CreateObject(cube_obj, "ceiling", shader, cam, ceiling_texure, 
                    glm::vec3(10.0f, 0.1f, 9.5f), glm::vec2(2.0f, 2.0f), WALL, 
                    glm::vec3(8.5f, 3.0f, 8.0f), g_VirtualScene); 


    TextRendering_Init();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Callbacks::updateDeltaTime();

        cam.processMouseMovement();
        shader.Use();

        g_Player.move(cam, g_VirtualScene.getObjects());
        g_Player.fly();
        g_Player.manageShooting(g_VirtualScene, cam, shader, g_VirtualScene.getObjects());

        weapon_sobj.setScale(glm::vec3(0.03f, 0.03f, 0.03f));
        weapon_sobj.setRotationY(90.0f);        
        
        enemy.move(g_VirtualScene.getObjects(), g_Player.getPosition());
        enemy.manageShooting(g_Player.getPosition(), g_VirtualScene, 
                             g_Player.getBBoxMin(), g_Player.getBBoxMax(), cam, shader, g_VirtualScene.getObjects());

        shader.Use();
        g_VirtualScene.drawScene();

        glm::vec4 p_model(0.0f, 0.0f, 0.0f, 1.0f);
        TextRendering_ShowModelViewProjection(window, cam.getProjectionMatrix(), cam.getViewMatrix(), Matrix_Identity(), p_model);
        TextRendering_ShowCameraInfo(window, cam, -1.0f, -0.5f);

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
    snprintf(buffer, 100, "Pos: (%.2f, %.2f, %.2f, %.1f)", cam.position->x, cam.position->y, cam.position->z, cam.position->w);
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

// Helper function to create and add an object to the scene
SceneObject* CreateObject(const ObjModel& cube_obj,
                        const std::string& name,
                        Shader& shader,
                        Camera& cam,
                        Texture3D& object_texture,
                        const glm::vec3& scale,
                        const glm::vec2& texture_scale,
                        int id,
                        const glm::vec3& position,
                        VirtualScene& scene)
{
    SceneObject* newObject = new SceneObject(cube_obj, name, shader, cam, true, true);
    newObject->setTexture(object_texture);
    newObject->setScale(scale);
    newObject->setTextureScale(texture_scale);
    newObject->setID(id);
    newObject->setPosition(position);
    scene.addObject(newObject);
    return newObject;
}