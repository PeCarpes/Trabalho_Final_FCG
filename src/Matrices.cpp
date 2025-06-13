#include "../include/Matrices.h"
#include <iostream>

glm::mat4 Matrix(
    float m00, float m01, float m02, float m03, // LINHA 1
    float m10, float m11, float m12, float m13, // LINHA 2
    float m20, float m21, float m22, float m23, // LINHA 3
    float m30, float m31, float m32, float m33  // LINHA 4
)
{
    return glm::mat4(
        m00, m10, m20, m30, // COLUNA 1
        m01, m11, m21, m31, // COLUNA 2
        m02, m12, m22, m32, // COLUNA 3
        m03, m13, m23, m33  // COLUNA 4
    );
}

glm::mat4 Matrix_Identity()
{
    return Matrix(
        1.0f , 0.0f , 0.0f , 0.0f , // LINHA 1
        0.0f , 1.0f , 0.0f , 0.0f , // LINHA 2
        0.0f , 0.0f , 1.0f , 0.0f , // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f   // LINHA 4
    );
}

glm::mat4 Matrix_Translate(float tx, float ty, float tz)
{
    return Matrix(
        // PREENCHA AQUI A MATRIZ DE TRANSLAÇÃO (3D) EM COORD. HOMOGÊNEAS
        // UTILIZANDO OS PARÂMETROS tx, ty e tz
        1.0f , 0.0f , 0.0f , tx,    // LINHA 1
        0.0f , 1.0f , 0.0f , ty,    // LINHA 2
        0.0f , 0.0f , 1.0f , tz,    // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f   // LINHA 4
    );
}

glm::mat4 Matrix_Scale(float sx, float sy, float sz)
{
    return Matrix(
        // PREENCHA AQUI A MATRIZ DE ESCALAMENTO (3D) EM COORD. HOMOGÊNEAS
        // UTILIZANDO OS PARÂMETROS sx, sy e sz
        sx,    0.0f , 0.0f , 0.0f ,  // LINHA 1
        0.0f , sy   , 0.0f , 0.0f ,  // LINHA 2
        0.0f , 0.0f , sz   , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

glm::mat4 Matrix_Rotate_X(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    // PREENCHA AQUI A MATRIZ DE ROTAÇÃO (3D) EM TORNO DO EIXO X EM COORD.
    return Matrix(
        // HOMOGÊNEAS, UTILIZANDO OS PARÂMETROS c e s
        1.0f ,  0.0f  , 0.0f, 0.0f ,  // LINHA 1
        0.0f ,  c     , s   , 0.0f ,  // LINHA 2
        0.0f , -s     , c   , 0.0f ,  // LINHA 3
        0.0f ,  0.0f  , 0.0f, 1.0f    // LINHA 4
    );
}

glm::mat4 Matrix_Rotate_Y(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return Matrix(
        // PREENCHA AQUI A MATRIZ DE ROTAÇÃO (3D) EM TORNO DO EIXO Y EM COORD.
        // HOMOGÊNEAS, UTILIZANDO OS PARÂMETROS c e s
        c    , 0.0f , -s   , 0.0f ,  // LINHA 1
        0.0f , 1.0f , 0.0f , 0.0f ,  // LINHA 2
        s    , 0.0f , c    , 0.0f ,  // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

glm::mat4 Matrix_Rotate_Z(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return Matrix(
        // PREENCHA AQUI A MATRIZ DE ROTAÇÃO (3D) EM TORNO DO EIXO Z EM COORD.
        // HOMOGÊNEAS, UTILIZANDO OS PARÂMETROS c e s
        c   , -s  , 0.0f , 0.0f ,  // LINHA 1
        s   ,  c  , 0.0f , 0.0f ,  // LINHA 2
        0.0f, 0.0f, 1.0f , 0.0f ,  // LINHA 3
        0.0f, 0.0f, 0.0f , 1.0f    // LINHA 4
    );
}

float norm(glm::vec4 v)
{
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    //return sqrt( vx*vx + /* PREENCHA AQUI o que falta para definir norma Euclidiana */ );
    return sqrt( vx*vx +  vy*vy + vz*vz);
}

glm::mat4 Matrix_Rotate(float angle, glm::vec4 axis)
{
    float c = cos(angle);
    float s = sin(angle);

    glm::vec4 v = axis / norm(axis);

    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return Matrix(
        // MATRIZ DE ROTAÇÃO (3D) EM TORNO DO EIXO v EM COORD.
        // HOMOGÊNEAS, UTILIZANDO OS PARÂMETROS vx, vy, vz, c e s (FÓRMULA DE RODRIGUES)
        vx*vx*(1-c) + c    , vx*vy*(1-c) - vz*s , vx*vz*(1-c) + vy*s , 0.0f ,  // LINHA 1
        vx*vy*(1-c) + vz*s , vy*vy*(1-c) + c    , vy*vz*(1-c) - vx*s , 0.0f ,  // LINHA 2
        vx*vz*(1-c) - vy*s , vy*vz*(1-c) + vx*s , vz*vz*(1-c) + c    , 0.0f ,  // LINHA 3
        0.0f               , 0.0f               , 0.0f               , 1.0f    // LINHA 4
    );
}

glm::vec4 crossproduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;

    return glm::vec4(
        u2*v3 - u3*v2, 
        u3*v1 - u1*v3, 
        u1*v2 - u2*v1,  
        0.0f // w = 0 para vetores.
    );
}


float dotproduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float u4 = u.w;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;
    float v4 = v.w;

    if ( u4 != 0.0f || v4 != 0.0f )
    {
        fprintf(stderr, "ERROR: Produto escalar não definido para pontos.\n");
        std::exit(EXIT_FAILURE);
    }

    return u1*v1 + u2*v2 + u3*v3;
}

glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector)
{
    glm::vec4 w = -view_vector;
    glm::vec4 u = crossproduct(up_vector, w);

    // Normalizamos os vetores u e w
    w = w / norm(w);
    u = u / norm(u);

    glm::vec4 v = crossproduct(w,u);

    glm::vec4 origin_o = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    float ux = u.x;
    float uy = u.y;
    float uz = u.z;
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;
    float wx = w.x;
    float wy = w.y;
    float wz = w.z;

    return Matrix(
        // MATRIZ DE MUDANÇA DE SISTEMA DE COORDENADAS (3D)
        // PARA AS COORDENADAS DE CÂMERA (MATRIZ VIEW HOMOGÊNEA), UTILIZANDO
        // OS PARÂMETROS ux,uy,uz, vx,vy,vz, wx,wy,wz, position_c, origin_o,
        // e a função dotproduct().
        //
        // ATENÇÃO: O produto escalar, computado pela função dotproduct(), está
        // definido somente para argumentos que são VETORES. Não existe produto
        // escalar de PONTOS.
        //
        ux    , uy,    uz   , dotproduct(-u, position_c - origin_o) ,  // LINHA 1
        vx    , vy   , vz   , dotproduct(-v, position_c - origin_o) ,  // LINHA 2
        wx    , wy   , wz   , dotproduct(-w, position_c - origin_o) ,  // LINHA 3
        0.0f  , 0.0f , 0.0f , 1.0f    // LINHA 4
    );
}

glm::mat4 Matrix_Orthographic(float l, float r, float b, float t, float n, float f)
{
    glm::mat4 M = Matrix(
        // PREENCHA AQUI A MATRIZ M DE PROJEÇÃO ORTOGRÁFICA (3D) UTILIZANDO OS
        // PARÂMETROS l,r,b,t,n,f 
        2/(r-l), 0.0f    , 0.0f    , -(r+l)/(r-l),  // LINHA 1
        0.0f   , 2/(t-b) , 0.0f    , -(t+b)/(t-b),  // LINHA 2
        0.0f   , 0.0f    , 2/(f-n) , -(f+n)/(f-n),  // LINHA 3
        0.0f   , 0.0f    , 0.0f    ,  1.0f    // LINHA 4
    );

    return M;
}


// Matriz de projeção perspectiva
glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f)
{
    float t = fabs(n) * tanf(field_of_view / 2.0f);
    float b = -t;
    float r = t * aspect;
    float l = -r;

    glm::mat4 P = Matrix(
        // PREENCHA AQUI A MATRIZ P DE PROJEÇÃO PERSPECTIVA (3D) UTILIZANDO OS
        // PARÂMETROS n e f.
        n   , 0.0f , 0.0f, 0.0f ,  // LINHA 1
        0.0f, n    , 0.0f, 0.0f ,  // LINHA 2
        0.0f, 0.0f , n+f , -f*n ,  // LINHA 3
        0.0f, 0.0f , 1.0f, 0.0f    // LINHA 4
    );

    // A matriz M é a mesma computada acima em Matrix_Orthographic().
    glm::mat4 M = Matrix_Orthographic(l, r, b, t, n, f);

    // Note que as matrizes M*P e -M*P fazem exatamente a mesma projeção
    // perspectiva, já que o sinal de negativo não irá afetar o resultado
    // devido à divisão por w.
    //
    // Então, por que utilizamos -M*P ao invés de M*P? Pois a especificação de
    // OpenGL define que os pontos fora do cubo unitário NDC deverão ser
    // descartados já que não irão aparecer na tela. O teste que define se um ponto
    // q está dentro do cubo unitário NDC pode ser expresso como:
    //
    //      -1 <= qx'/w <= 1   &&  -1 <= qy'/w <= 1   &&  -1 <= qz'/w <= 1
    //
    // ou, de maneira equivalente SE w > 0, a placa de vídeo faz o seguinte teste
    // ANTES da divisão por w:
    //
    //      -w <= qx' <= w   &&  -w <= qy' <= w   &&  -w <= qz' <= w
    //
    // Note que o teste acima economiza uma divisão por w caso o ponto seja
    // descartado (quando esteja fora de NDC), entretanto, este último teste só
    // é equivalente ao primeiro teste SE E SOMENTE SE w > 0 (isto é, se w for
    // positivo). Como este último teste é o que a placa de vídeo (GPU) irá fazer,
    // precisamos utilizar a matriz -M*P para projeção perspectiva, de forma que
    // w seja positivo.
    //
    return -M*P;
}

void PrintMatrix(glm::mat4 M)
{
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3]);
}
