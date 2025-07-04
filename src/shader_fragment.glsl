#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_model;
in vec4 position_world;
in vec4 normal;
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define WEAPON 0
#define BUNNY 1
#define ENEMY 2
#define CUBE 3
#define WALL 4

uniform vec3 bbox_min;
uniform vec3 bbox_max;

uniform int object_id;
uniform sampler2D TextureImage;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

float U = 0.0;
float V = 0.0;

void calculate_planar_UV(){
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        U = (position_model.x - minx) / (maxx - minx);
        V = (position_model.y - miny) / (maxy - miny);
}

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal
    vec4 r = -l + 2*n*(dot(n, l)); // PREENCHA AQUI o vetor de reflexão especular ideal

    vec3 texture_color;

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd;   // Refletância difusa
    vec3 Ks;   // Refletância especular
    vec3 Ka;   // Refletância ambiente
    float q;   // Expoente especular para o modelo de iluminação de Phong
    
    if (object_id == WEAPON)
    {
        Ks = vec3(0.5, 0.5, 0.5);
        Ka = Ks * 0.0;
        Kd = texture(TextureImage, texcoords).rgb;   // Refletância difusa
        q = 64.0;
    }
    else if (object_id == BUNNY)
    {
        Ks = vec3(0.8, 0.8, 0.8);
        Ka = Ks * 0.2;
        vec3 Kd = texture(TextureImage, texcoords).rgb;   // Refletância difusa
        q = 32.0;
    }
    else if (object_id == ENEMY)
    {
        Kd = vec3(0.8, 0.0, 0.0);
        Ks = vec3(0.0, 0.0, 0.0);
        Ka = vec3(0.2, 0.0, 0.0);
        vec3 Kd = texture(TextureImage, texcoords).rgb;   // Refletância difusa
        q = 1.0;
    }
    else if (object_id == CUBE)
    {
        // In order to apply a texture to the TOP of the cube,
        // we need to calculate the UV coordinates based on the xz plane
        vec3 diag = bbox_max - bbox_min;
        vec3 local = (position_model.xyz - bbox_min) / diag;
        vec2 uv = local.xz; 

        Ks = vec3(0.0, 0.0, 0.0);
        Ka = Ks * 0.2;
        Kd = texture(TextureImage, uv).rgb; // Refletância difusa
        q = 1.0;
    }
    else if (object_id == WALL)
    {
        vec3 Kd = texture(TextureImage, texcoords).rgb;   // Refletância difusa
        Ks = vec3(0.0, 0.0, 0.0);
        Ka = vec3(0.2, 0.2, 0.2);
        q = 1.0;
    }
    else
    {
        Kd = vec3(0.8, 0.4, 0.08);
        Ks = vec3(0.0, 0.0, 0.0);
        Ka = vec3(1.0, 0.2, 0.04);
        q = 1.0;
    }

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0, 1.0, 1.0); // PREENCH AQUI o espectro da fonte de luz

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2,0.2,0.2); // PREENCHA AQUI o espectro da luz ambiente

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd*I*max(0, dot(n, l)); // PREENCHA AQUI o termo difuso de Lambert

    // Termo ambiente
    vec3 ambient_term =  Ka * Ia; // PREENCHA AQUI o termo ambiente

    // Termo especular utilizando o modelo de iluminação de Phong
    vec3 phong_specular_term  = Ks*I*pow(max(0, dot(r, v)), q); // PREENCH AQUI o termo especular de Phong

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final do fragmento calculada com uma combinação dos termos difuso,
    // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.

    color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
} 

