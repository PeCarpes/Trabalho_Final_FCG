#version 330 core

layout (location = 0) in vec4 model_coefficients;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Exatamente o mesmo que o seu shader principal, só projeta o vértice.
    gl_Position = projection * view * model * model_coefficients;
}