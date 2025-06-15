#version 330 core

layout (location = 0) in vec4 model_coefficients;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Saída especial para o nosso debug
out vec4 v_clip_space_pos;

void main()
{
    // A transformação MVP normal
    gl_Position = projection * view * model * model_coefficients;
    
    // Passamos a posição final (em Clip Space) para o Fragment Shader
    v_clip_space_pos = gl_Position;
}