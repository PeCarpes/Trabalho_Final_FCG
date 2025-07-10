# Trabalho final da cadeira de fundamentos da computação gráfica UFRGS 2025/1
A aplicação consiste em um jogo desenvolvido com a utilização de OpenGL e escrito em C++. É um FPS chamado "Sufficiently Warm", onde o principal objetivo do jogador é chegar ao final da fase sem ser atingido pelos projéteis dos inimigos. A principal mecânica do jogo é o controle do tempo baseado na movimentação do jogador. Isso significa que, caso o jogador pare de se mover, a velocidade de passagem do tempo irá diminuir gradualmente (até um certo limite), e voltará a aumentar até 100%, assim que este volte a andar. O jogo que serviu de inspiração para implementar essa mecânica foi o [SuperHot](https://superhotgame.com).    

## Divisão de tarefas:
**Gabriel**:
- Estruturação inicial dos módulos e classes do projeto
- Funções de tratamento para desenho de objetos 3d no cenário
- Curvas de bézier
- Lógica dos inimigos
- Detecção e tratamento das colisões
- Lógica dos tiros e remoção dos objetos do cenário
- Efeitos sonoros
  
**Pedro**:
- Funcionamento da câmera
- Tratamento de callbacks do teclado e mouse
- Introdução e tratamento das texturas e iluminação
- Design e aplicação do mapa
- Lógica dos estados do jogo (menu, vitória, derrota)
- Câmera Look-at para o projétil

## Uso de IAs

O Github Copilot foi usado extensivamente para realizar _autocomplete_ do código, especialmente em trechos repetitivos, como a definição de _getters_ e _setters_ de classes, mas não para implementar a lógica do jogo.

O ChatGPT e o Gemini foram utilizados tanto para sugerir maneiras de implementar algumas funcionalidades (como o tratamento de colisões), quanto para auxiliar no _debug_ do projeto, mas nenhum trecho de código foi copiado dessas ferramentas.  

## Critérios de Avaliação
- Malhas poligonais complexas: Modelo dos inimigos, da arma e do olho no fim da fase.
- Transformações geométricas controladas pelo usuário: Movimentação da bounding-box do jogador, além de transformações indiretas como o alinhamento dos projéteis em relação à câmera, etc.
- Tipos de Câmera:
  - A câmera do jogador é uma câmera livre. 
  - Câmera look-at que segue o último tiro disparado pelo jogador.
- Instâncias de objetos: Tanto paredes, quanto o teto e o chão são instâncias de um mesmo objeto (cube.obj). Além disso, os tiros e os inimigos são múltiplas instâncias de seus respectivos objetos.
- Três tipos de testes de intersecção: 
  - **ponto-prisma**, o mapa é dividido em 3 sub-áreas. É usado um teste de intesecção da posição do jogador com cada uma dessas sub-áreas para saber quais inimigos devem ser acionados. Também, para saber se o jogador está na linha de visão de um inimigo, é traçada uma linha entre eles, sobre a qual são gerados vários pontos utilizados em testes de intersecção com os objetos da cena.  
  - **prisma-prisma** é utilizado para detectar a colisão do jogador e dos inimigos com os objetos do mapa.
  - **prisma-esfera** é utilizado para detecção dos projéteis com os outros objetos (jogador, inimigos, paredes, etc.).
- Modelos de Iluminação Difusa e Blinn-Phong: As paredes e os projéteis usam iluminação difusa. A arma, os inimigos e o olho final usam iluminação de blinn-phong.
- Modelos de Interpolação de Phong e Gouraud: Quase todos os objetos desenhados na cena são interpolados com o modelo de phong, exceto os projéteis, que utilizam Gourad.
- Mapeamento de texturas em todos os objetos: Quase todos os objetos possuem textura, a única execção são os inimigos, pois o estilo de arte do jogo no qual nos baseamos não utiliza texturas para eles. Em vez disso, são coloridos de vermelho sólido.
- Movimentação com curva Bézier cúbica: O movimento da arma do jogador ao andar (bobbing) é definido por uma curva de bézier cúbica.
- Animações baseadas no tempo ($\Delta t$): Todas animações são baseadas no tempo, a única diferença é que elas são modificadas por um coeficiente controlado pela velocidade do jogador.

## Manual:

### Movimento do Jogador

| Tecla | Ação | Descrição |
| -------- | ------- | ------- |
| W | Andar para frente | |
| A | Andar para a esquerda | |
| S | Andar para trás | |
| D | Andar par a direita | |
|Shift| Correr| Dobra a Velocidade do Jogador|
| F | Ativar "God Mode" | Jogador fica Imortal
| Q | Seguir último disparo | Ativa a câmera Lookat
| R | Reinicia o Jogo | Objetos voltam à posição original

### Imagens:
![image](https://github.com/user-attachments/assets/c7af83aa-c447-4465-9ed8-ddba5fdd01ca)
![image](https://github.com/user-attachments/assets/900ca8a2-b375-4e24-8e7f-4b87411c6506)


## Como Compilar:

### Para Linux Ubuntu

1. Instale as depenências:

```
apt-get update -y && apt-get upgrade -y && apt-get install -y --no-install-recommends build-essential make libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxcb1-dev libxext-dev libxrender-dev libxfixes-dev libxau-dev libglu1-mesa-dev libxdmcp-dev libxxf86vm-dev  libgl1-mesa-dev libglm-dev
```

2. Gere o Makefile com CMake:

`cmake .`

3. Rode o Makefile:
  
`make run`

### Com a extensão Dev Containers

1. Instale a extensão Dev Containers no VSCode.
2. Aperta `Ctrl + Shift + P` e selecione: `Dev Containers: Open Folder in Container`
3. Espere o container ser gerado (pode demorar um pouco)

4. Para compilar para Linux, simplesmente escreva `cmake .` para gerar o Makefile, e `make` para compilar.
  
5. Para compilar para Windows, rode `sudo chmod +x ./build_windows.sh && ./build_windows.sh && make` (dentro do Container).
