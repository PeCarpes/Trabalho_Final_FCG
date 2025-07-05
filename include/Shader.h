#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <utils.h>

class Shader{
    private:
        GLuint GpuProgramID; 
        GLuint vertex_shader_id;
        GLuint fragment_shader_id;
        
        void LoadShadersFromFiles();
        GLuint LoadShader_Vertex(const char *filename);
        GLuint LoadShader_Fragment(const char *filename);
        void LoadShader(const char *filename, GLuint shader_id);
            
        public:
        Shader();
        Shader(GLuint vertex_shader_id, GLuint fragment_shader_id);
        void Use();
        
        GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
        GLuint GetGpuProgramID() const { return GpuProgramID; }
        GLuint GetVertexShaderID() const { return vertex_shader_id; }
        GLuint GetFragmentShaderID() const { return fragment_shader_id; }

        void SetUniform(const char *name, const glm::mat4 &matrix)const;
        void SetUniform(const char *name, const glm::vec3 &vector)const;
        void SetUniform(const char *name, const glm::vec4 &vector)const;
        void SetUniform(const char *name, float value)const;
        void SetUniform(const char *name, int value)const;
        void SetUniform(const char *name, const glm::vec2 &vector)const;

        void ClearZBuffer() const { glClear(GL_DEPTH_BUFFER_BIT); }
};