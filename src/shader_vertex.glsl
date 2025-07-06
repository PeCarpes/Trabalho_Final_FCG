#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTrianglesAndAddToVirtualScene() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int object_id;
uniform sampler2D TextureImage;

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;
out vec3 gouraud_vertex_color;

#define WEAPON 0
#define ENEMY 1
#define CUBE 2
#define WALL 3
#define DEBUG 4
#define PROJECTILE 5 // !! ATUALIZAR ID DENTRO DE Projectile.cpp CASO MUDAR AQUI !!

void main()
{
    // A variável gl_Position define a posição final de cada vértice
    // OBRIGATORIAMENTE em "normalized device coordinates" (NDC), onde cada
    // coeficiente estará entre -1 e 1 após divisão por w.
    // Veja {+NDC2+}.
    //
    // O código em "main.cpp" define os vértices dos modelos em coordenadas
    // locais de cada modelo (array model_coefficients). Abaixo, utilizamos
    // operações de modelagem, definição da câmera, e projeção, para computar
    // as coordenadas finais em NDC (variável gl_Position). Após a execução
    // deste Vertex Shader, a placa de vídeo (GPU) fará a divisão por W. Veja
    // slides 41-67 e 69-86 do documento Aula_09_Projecoes.pdf.

    gl_Position = projection * view * model * model_coefficients;

    // Como as variáveis acima  (tipo vec4) são vetores com 4 coeficientes,
    // também é possível acessar e modificar cada coeficiente de maneira
    // independente. Esses são indexados pelos nomes x, y, z, e w (nessa
    // ordem, isto é, 'x' é o primeiro coeficiente, 'y' é o segundo, ...):
    //
    //     gl_Position.x = model_coefficients.x;
    //     gl_Position.y = model_coefficients.y;
    //     gl_Position.z = model_coefficients.z;
    //     gl_Position.w = model_coefficients.w;
    //

    // Agora definimos outros atributos dos vértices que serão interpolados pelo
    // rasterizador para gerar atributos únicos para cada fragmento gerado.

    // Posição do vértice atual no sistema de coordenadas global (World).
    position_world = model * model_coefficients;
    
    
    if(object_id == PROJECTILE) // aplica gouraud shading se for um projetil
    {
        vec4 normal_world = normalize(inverse(transpose(model)) * normal_coefficients);

        vec4 camera_position = inverse(view)[3];
        vec3 n = normal_world.xyz;
        vec3 v = normalize(camera_position.xyz - position_world.xyz);
        vec3 l = normalize(vec3(1.0, 1.0, 0.5));

        // Modelo Blinn-Phong
        vec3 h = normalize(v + l);

        // Pega a normal da superfície e a torna positiva.
        vec3 blend_weights = abs(n.xyz);

        // "Suaviza" os pesos para evitar transições duras entre as faces.
        // Um valor de 'power' maior torna a transição mais nítida.
        float power = 2.0;
        blend_weights = pow(blend_weights, vec3(power));
        blend_weights = normalize(blend_weights);

        // Calcula as três projeções de textura possíveis.
        vec2 uv_top_down   = position_world.xz; // Projeção de cima (XZ)
        vec2 uv_front_back = position_world.xy; // Projeção da frente (XY)
        vec2 uv_left_right = position_world.zy; // Projeção do lado (ZY)

        // Lê a cor da textura para cada uma das três projeções.
        vec3 top_down_color   = texture(TextureImage, uv_top_down).rgb;
        vec3 front_back_color = texture(TextureImage, uv_front_back).rgb;
        vec3 left_right_color = texture(TextureImage, uv_left_right).rgb;

        // Mistura as três cores de textura usando os pesos da normal.
        vec3 triplanar_color = top_down_color * blend_weights.y +    // Projeção de cima (Y)
                                front_back_color * blend_weights.z +  // Projeção da frente (Z)
                                left_right_color * blend_weights.x;   // Projeção do lado (X)

        vec3 Kd = triplanar_color;

        // Cor difusa da textura
        // vec3 Kd = texture(TextureImage, texture_coefficients).rgb;

        vec3 Ks = vec3(1.0, 1.0, 1.0); // Cor especular
        vec3 Ka = vec3(0.1, 0.1, 0.1); // Cor ambiente
        float q = 1.0f; // Exponente de brilho

        vec3 I = vec3(1.0, 1.0, 1.0);
        vec3 Ia = vec3(0.2, 0.2, 0.2);

        // Cálculo final da cor PARA ESTE VÉRTICE
        vec3 lambert_diffuse = Kd * I * max(0, dot(n, l));
        vec3 blinn_phong_specular = Ks * I * pow(max(0, dot(n, h)), q);
        vec3 ambient = Ka * Ia;

        // A cor final é enviada para o rasterizador
        gouraud_vertex_color = lambert_diffuse + blinn_phong_specular + ambient;
    }
    else
    {
        position_model = model_coefficients;

        // Normal do vértice atual no sistema de coordenadas global (World).
        // Veja slides 123-151 do documento Aula_07_Transformacoes_Geometricas_3D.pdf.
        normal = inverse(transpose(model)) * normal_coefficients;
        normal.w = 0.0;

        // Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
        texcoords = texture_coefficients;
    }
}

