#version 330 core

// Uma cor que vamos enviar do C++
uniform vec3 u_color;

out vec4 color;

void main()
{
    // A cor do fragmento é simplesmente a cor que enviamos, sem iluminação.
    color = vec4(u_color, 1.0);
}