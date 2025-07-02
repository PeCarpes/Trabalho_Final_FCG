#include <Shader.h>

Shader::Shader(GLuint vertex_shader_id, GLuint fragment_shader_id)
    : GpuProgramID(0), vertex_shader_id(vertex_shader_id), fragment_shader_id(fragment_shader_id)
{
    // Deletamos o programa de GPU anterior, caso ele exista.
    if (GpuProgramID != 0)
        glDeleteProgram(GpuProgramID);

    // Criamos um programa de GPU utilizando os shaders previamente carregados.
    GpuProgramID = CreateGpuProgram(vertex_shader_id, fragment_shader_id);
}

Shader::Shader()
{
    LoadShadersFromFiles();
}

void Shader::Use()
{
    glUseProgram(GpuProgramID);
}

void Shader::SetUniform(const char *name, const glm::mat4 &matrix) const
{
    GLint location = glGetUniformLocation(GpuProgramID, name);
    if (location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    else
    {
        fprintf(stderr, "ERROR: Uniform \"%s\" not found in shader program.\n", name);
    }

}

void Shader::SetUniform(const char *name, const glm::vec3 &vector)  const
{
    GLint location = glGetUniformLocation(GpuProgramID, name);
    if (location != -1)
    {
        glUniform3fv(location, 1, glm::value_ptr(vector));
    }
    else
    {
        fprintf(stderr, "ERROR: Uniform \"%s\" not found in shader program.\n", name);
    }
}

void Shader::SetUniform(const char *name, const glm::vec4 &vector) const
{
    GLint location = glGetUniformLocation(GpuProgramID, name);
    if (location != -1)
    {
        glUniform4fv(location, 1, glm::value_ptr(vector));
    }
    else
    {
        fprintf(stderr, "ERROR: Uniform \"%s\" not found in shader program.\n", name);
    }
}

void Shader::SetUniform(const char *name, float value) const
{
    GLint location = glGetUniformLocation(GpuProgramID, name);
    if (location != -1)
    {
        glUniform1f(location, value);
    }
    else
    {
        fprintf(stderr, "ERROR: Uniform \"%s\" not found in shader program.\n", name);
    }
}

void Shader::SetUniform(const char *name, int value) const
{
    GLint location = glGetUniformLocation(GpuProgramID, name);
    if (location != -1)
    {
        glUniform1i(location, value);
    }
    else
    {
        fprintf(stderr, "ERROR: Uniform \"%s\" not found in shader program.\n", name);
    }
}

void Shader::LoadShader(const char *filename, GLuint shader_id)
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

GLuint Shader::CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
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
GLuint Shader::LoadShader_Fragment(const char *filename)
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
GLuint Shader::LoadShader_Vertex(const char *filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

void Shader::LoadShadersFromFiles()
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
    vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    fragment_shader_id = LoadShader_Fragment("../../src/shader_fragment.glsl");

    // Criamos um programa de GPU utilizando os shaders carregados acima.
    GpuProgramID = CreateGpuProgram(vertex_shader_id, fragment_shader_id);
}
