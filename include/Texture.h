#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <stdexcept>

#define MAX_TEXTURES 16

class Texture3D{

private:
    static int num_textures;
    static int num_loaded_textures;
    
    GLuint sampler_id;
    GLuint texture_id;

public:
    void LoadTextureImage(const char* filename);
    
    GLuint getTextureID() const { return texture_id; }

};
