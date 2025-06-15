#version 330 core

// Recebemos a posição final do vértice, interpolada para este fragmento
in vec4 v_clip_space_pos;

out vec4 color;

void main()
{
    // 1. Converte de Clip Space para NDC (Normalized Device Coordinates)
    // As coordenadas NDC vão de -1.0 a 1.0 em cada eixo
    vec3 ndc = v_clip_space_pos.xyz / v_clip_space_pos.w;

    // 2. Mapeia as coordenadas de [-1, 1] para o intervalo de cores [0, 1]
    // O eixo X vira Vermelho (R), Y vira Verde (G), Z vira Azul (B)
    vec3 final_color = (ndc + 1.0) / 2.0;

    // 3. Pinta o fragmento com essa cor
    color = vec4(final_color, 1.0);
}