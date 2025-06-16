
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering_ShowCameraInfo(GLFWwindow* window, Camera& cam, float x, float y);
void TextRendering_ShowModelViewProjection(GLFWwindow* window, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 p_model);
void TextRendering_ShowProjection(GLFWwindow* window);
void TextRendering_ShowFramesPerSecond(GLFWwindow* window);

void LoadShadersFromFiles();
void LoadShader(const char *filename, GLuint shader_id);
GLuint LoadShader_Fragment(const char *filename);
GLuint LoadShader_Vertex(const char *filename);
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);

const int screen_width = 1200;
const int screen_height = 800;
float aspect_ratio = (float)screen_width / (float)screen_height;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
GLuint g_GpuProgramID = 0;
GLuint g_DebugGpuProgramID = 0; // TEMP --------------
GLuint g_DebugCubeVAO = 0; // TEMP --------------
GLint g_model_uniform;
GLint g_view_uniform;
GLint g_projection_uniform;
GLint g_object_id_uniform;

VirtualScene g_VirtualScene;

bool g_ShowInfoText = true;

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

    LoadShadersFromFiles();

    // TEMP -------------
    GLuint debug_vertex_shader_id = LoadShader_Vertex("../../src/debug_shader_vertex.glsl");
    GLuint debug_fragment_shader_id = LoadShader_Fragment("../../src/debug_shader_fragment.glsl");
    g_DebugGpuProgramID = CreateGpuProgram(debug_vertex_shader_id, debug_fragment_shader_id);
    // TEMP -------------

    GLfloat triangle_vertices[] = {
        // Posição (x, y, z)
        -0.5f, -0.5f, 0.0f, // Vértice 0
        0.5f, -0.5f, 0.0f,  // Vértice 1
        0.0f, 0.5f, 0.0f    // Vértice 2
    };

    GLuint triangle_indices[] = {
        0, 1, 2 // Triângulo formado pelos vértices 0, 1 e 2
    };

    int num_vertices = sizeof(triangle_vertices) / (3 * sizeof(GLfloat));
    int num_indices = sizeof(triangle_indices) / sizeof(GLuint);

    ObjModel bunny_obj("../../data/bunny.obj");
    bunny_obj.ComputeNormals();
    bunny_obj.BuildTriangles();

    auto bunny_sobj = std::make_shared<SceneObject>(bunny_obj, g_GpuProgramID);

    g_VirtualScene.addObject(bunny_sobj);

    // TEMP -------------
    GLfloat debug_cube_vertices[] = {
        // Posições (x, y, z)
        -10.0f, -10.0f, -10.0f, // 0
         10.0f, -10.0f, -10.0f, // 1
         10.0f,  10.0f, -10.0f, // 2
        -10.0f,  10.0f, -10.0f, // 3
        -10.0f, -10.0f,  10.0f, // 4
         10.0f, -10.0f,  10.0f, // 5
         10.0f,  10.0f,  10.0f, // 6
        -10.0f,  10.0f,  10.0f  // 7
    };

    GLuint debug_cube_indices[] = {
        // Face de trás
        0, 1, 2,  2, 3, 0,
        // Face da frente
        4, 5, 6,  6, 7, 4,
        // Face da esquerda
        4, 7, 3,  3, 0, 4,
        // Face da direita
        5, 6, 2,  2, 1, 5,
        // Face de baixo
        4, 5, 1,  1, 0, 4,
        // Face de cima
        7, 6, 2,  2, 3, 7
    };

    GLuint vbo, ebo;
    glGenVertexArrays(1, &g_DebugCubeVAO);
    glBindVertexArray(g_DebugCubeVAO);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(debug_cube_vertices), debug_cube_vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(debug_cube_indices), debug_cube_indices, GL_STATIC_DRAW);

    // O atributo do vértice (posição) é o layout location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Desvincula o VAO
    // TEMP -------------

    Camera cam(glm::vec4(0.0f, 0.0f, 3.0f, 1.0f));
    bool freecam = true; // Uma variável para controlar o modo
    glm::vec4 lookat_pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    TextRendering_Init();

    glm::vec2 mouse_pos = glm::vec2 (0, 0);
    glm::vec2 last_mouse_pos = glm::vec2 (0, 0);
    glm::vec2 mouse_offset = glm::vec2 (0, 0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Linha movida TEMPORARIAMENTE para baixo:
        // glUseProgram(g_GpuProgramID);

        last_mouse_pos = mouse_pos;
        mouse_pos = Callbacks::getCursorPosition();
        mouse_offset = mouse_pos - last_mouse_pos;


        glm::mat4 model = Matrix_Identity();

        if (freecam) {
            // --- Controle de Câmera Livre ---
            // Verifique o teclado (ex: if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS))
            // e chame minhaCamera.processKeyboard(CameraMovement::FORWARD, delta_time);
            
            // Obtenha a variação do mouse (x_offset, y_offset)
            // e chame minhaCamera.processMouseMovement(x_offset, y_offset);
            if(Callbacks::isLeftMouseButtonPressed())
            {
                cam.processMouseMovement(mouse_offset.x, mouse_offset.y);
            }

        } else {
            // --- Controle de Look-At ---
            // A câmera vai sempre olhar para o alvo.
            // Você pode até mover o alvo para testar. ex: posicao_do_alvo.x = sin(glfwGetTime());
            cam.lookAt(lookat_pos);


        }

        // Obs:
        // Para modificar o coelho agora, usar o operador '->'
        // Exemplo: bunny_sobj->setTranslationMatrix(...);

        // bunny_sobj->setTranslationMatrix(glm::translate(model, glm::vec3
        //                    (Callbacks::getCursorPosition().x * 0.01f - 6, 
        //                     4 - Callbacks::getCursorPosition().y * 0.01f, 
        //                     0.0f)));
        float field_of_view = 3.141592 / 3.0f;
        aspect_ratio = Callbacks::getScreenRatio();
        glm::mat4 view = cam.getViewMatrix();
        // glm::mat4 projection = cam.getProjectionMatrix(aspect_ratio);
        glm::mat4 projection = glm::perspective(field_of_view, aspect_ratio, 0.1f, 100.0f); // Linha temp

         // --- BLOCO DE CÓDIGO PARA DESENHAR O CUBO DE DEBUG ---
    {
        glUseProgram(g_DebugGpuProgramID);

        // Truque para ver o interior do cubo: desabilitar o backface culling
        glDisable(GL_CULL_FACE);

        // Matrizes para o cubo
        glm::mat4 cube_model = Matrix_Identity(); // O cubo já é grande, não precisa mover
        glUniformMatrix4fv(glGetUniformLocation(g_DebugGpuProgramID, "model"), 1, GL_FALSE, glm::value_ptr(cube_model));
        glUniformMatrix4fv(glGetUniformLocation(g_DebugGpuProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(g_DebugGpuProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(g_DebugCubeVAO);

        // Cores para cada face
        glm::vec3 colors[] = {
            {1.0f, 0.0f, 0.0f}, // Vermelho - Trás
            {0.0f, 1.0f, 0.0f}, // Verde - Frente
            {0.0f, 0.0f, 1.0f}, // Azul - Esquerda
            {1.0f, 1.0f, 0.0f}, // Amarelo - Direita
            {1.0f, 0.0f, 1.0f}, // Magenta - Baixo
            {0.0f, 1.0f, 1.0f}  // Ciano - Cima
        };
        
        GLuint color_loc = glGetUniformLocation(g_DebugGpuProgramID, "u_color");

        // Desenha cada face (6 vértices por face) com uma cor diferente
        for (int i = 0; i < 6; ++i) {
            glUniform3fv(color_loc, 1, glm::value_ptr(colors[i]));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
        }

        glBindVertexArray(0);
        glEnable(GL_CULL_FACE); // Reabilita o culling para o resto da cena
    }
    // --- FIM DO BLOCO DO CUBO DE DEBUG ---


        glUseProgram(g_GpuProgramID);

        GLuint view_loc = glGetUniformLocation(g_GpuProgramID, "view");
        GLuint proj_loc = glGetUniformLocation(g_GpuProgramID, "projection");

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));

        g_VirtualScene.drawScene();

        glm::vec4 p_model(0.0f, 0.0f, 0.0f, 1.0f);
        TextRendering_ShowModelViewProjection(window, projection, view, model, p_model);
        TextRendering_ShowCameraInfo(window, cam, -1.0f, -0.5f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Função auxilar, utilizada pelas duas funções acima. Carrega código de GPU de
// um arquivo GLSL e faz sua compilação.
void LoadShader(const char *filename, GLuint shader_id)
{
    // Lemos o arquivo de texto indicado pela variável "filename"
    // e colocamos seu conteúdo em memória, apontado pela variável
    // "shader_string".
    std::ifstream file;
    try
    {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    }
    catch (std::exception &e)
    {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar *shader_string = str.c_str();
    const GLint shader_string_length = static_cast<GLint>(str.length());

    // Define o código do shader GLSL, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    // Compila o código do shader GLSL (em tempo de execução)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compilação
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos memória para guardar o log de compilação.
    // A chamada "new" em C++ é equivalente ao "malloc()" do C.
    GLchar *log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compilação
    if (log_length != 0)
    {
        std::string output;

        if (!compiled_ok)
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    // A chamada "delete" em C++ é equivalente ao "free()" do C
    delete[] log;
}

// Esta função cria um programa de GPU, o qual contém obrigatoriamente um
// Vertex Shader e um Fragment Shader.
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    // Criamos um identificador (ID) para este programa de GPU
    GLuint program_id = glCreateProgram();

    // Definição dos dois shaders GLSL que devem ser executados pelo programa
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    // Linkagem dos shaders acima ao programa
    glLinkProgram(program_id);

    // Verificamos se ocorreu algum erro durante a linkagem
    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    // Imprime no terminal qualquer erro de linkagem
    if (linked_ok == GL_FALSE)
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar *log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "== End of link log\n";

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete[] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Retorna o ID gerado acima
    return program_id;
}

// Carrega um Fragment Shader de um arquivo GLSL . Veja definição de LoadShader() abaixo.
GLuint LoadShader_Fragment(const char *filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}

// Carrega um Vertex Shader de um arquivo GLSL. Veja definição de LoadShader() abaixo.
GLuint LoadShader_Vertex(const char *filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

void LoadShadersFromFiles()
{
    // Note que o caminho para os arquivos "shader_vertex.glsl" e
    // "shader_fragment.glsl" estão fixados, sendo que assumimos a existência
    // da seguinte estrutura no sistema de arquivos:
    //
    //    + FCG_Lab_01/
    //    |
    //    +--+ bin/
    //    |  |
    //    |  +--+ Release/  (ou Debug/ ou Linux/)
    //    |     |
    //    |     o-- main.exe
    //    |
    //    +--+ src/
    //       |
    //       o-- shader_vertex.glsl
    //       |
    //       o-- shader_fragment.glsl
    //
    GLuint vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id = LoadShader_Fragment("../../src/shader_fragment.glsl");

    // Deletamos o programa de GPU anterior, caso ele exista.
    if (g_GpuProgramID != 0)
        glDeleteProgram(g_GpuProgramID);

    // Criamos um programa de GPU utilizando os shaders carregados acima.
    g_GpuProgramID = CreateGpuProgram(vertex_shader_id, fragment_shader_id);
}

void TextRendering_ShowCameraInfo(GLFWwindow* window, Camera& cam, float x, float y)
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
}

// Esta função recebe um vértice com coordenadas de modelo p_model e passa o
// mesmo por todos os sistemas de coordenadas armazenados nas matrizes model,
// view, e projection; e escreve na tela as matrizes e pontos resultantes
// dessas transformações.
void TextRendering_ShowModelViewProjection(
    GLFWwindow* window,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::vec4 p_model
)
{
    if ( !g_ShowInfoText )
        return;

    glm::vec4 p_world = model*p_model;
    glm::vec4 p_camera = view*p_world;
    glm::vec4 p_clip = projection*p_camera;
    glm::vec4 p_ndc = p_clip / p_clip.w;

    float pad = TextRendering_LineHeight(window);

    TextRendering_PrintString(window, " Model matrix             Model     In World Coords.", -1.0f, 1.0f-pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, model, p_model, -1.0f, 1.0f-2*pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f-6*pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f-7*pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f-8*pad, 1.0f);

    TextRendering_PrintString(window, " View matrix              World     In Camera Coords.", -1.0f, 1.0f-9*pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, view, p_world, -1.0f, 1.0f-10*pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f-14*pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f-15*pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f-16*pad, 1.0f);

    TextRendering_PrintString(window, " Projection matrix        Camera                    In NDC", -1.0f, 1.0f-17*pad, 1.0f);
    TextRendering_PrintMatrixVectorProductDivW(window, projection, p_camera, -1.0f, 1.0f-18*pad, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glm::vec2 a = glm::vec2(-1, -1);
    glm::vec2 b = glm::vec2(+1, +1);
    glm::vec2 p = glm::vec2( 0,  0);
    glm::vec2 q = glm::vec2(width, height);

    glm::mat4 viewport_mapping = Matrix(
        (q.x - p.x)/(b.x-a.x), 0.0f, 0.0f, (b.x*p.x - a.x*q.x)/(b.x-a.x),
        0.0f, (q.y - p.y)/(b.y-a.y), 0.0f, (b.y*p.y - a.y*q.y)/(b.y-a.y),
        0.0f , 0.0f , 1.0f , 0.0f ,
        0.0f , 0.0f , 0.0f , 1.0f
    );

    TextRendering_PrintString(window, "                                                       |  ", -1.0f, 1.0f-22*pad, 1.0f);
    TextRendering_PrintString(window, "                            .--------------------------'  ", -1.0f, 1.0f-23*pad, 1.0f);
    TextRendering_PrintString(window, "                            V                           ", -1.0f, 1.0f-24*pad, 1.0f);

    TextRendering_PrintString(window, " Viewport matrix           NDC      In Pixel Coords.", -1.0f, 1.0f-25*pad, 1.0f);
    TextRendering_PrintMatrixVectorProductMoreDigits(window, viewport_mapping, p_ndc, -1.0f, 1.0f-26*pad, 1.0f);
}

// Escrevemos na tela qual matriz de projeção está sendo utilizada.
void TextRendering_ShowProjection(GLFWwindow* window)
{
    if ( !g_ShowInfoText )
        return;

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);
}

// Escrevemos na tela o número de quadros renderizados por segundo (frames per
// second).
void TextRendering_ShowFramesPerSecond(GLFWwindow* window)
{
    if ( !g_ShowInfoText )
        return;

    // Variáveis estáticas (static) mantém seus valores entre chamadas
    // subsequentes da função!
    static float old_seconds = (float)glfwGetTime();
    static int   ellapsed_frames = 0;
    static char  buffer[20] = "?? fps";
    static int   numchars = 7;

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;

    if ( ellapsed_seconds > 1.0f )
    {
        numchars = snprintf(buffer, 20, "%.2f fps", ellapsed_frames / ellapsed_seconds);
    
        old_seconds = seconds;
        ellapsed_frames = 0;
    }

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, buffer, 1.0f-(numchars + 1)*charwidth, 1.0f-lineheight, 1.0f);
}