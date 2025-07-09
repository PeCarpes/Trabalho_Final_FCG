# Trabalho_Final_FCG
Trabalho final da cadeira de fundamentos da computação gráfica UFRGS 2025/1
A aplicação consiste de um jogo desenvolvido primariamente em OpenGL escrito em C++, o jogo chama-se "SUFFICIENTLY WARM" e consiste somente de uma fase cujo o objetivo é atingir o objetivo final sem ser atingido por nenhum projétil inimigo. A mecânica principal do jogo é o controle do tempo baseado na velocidade do jogador, a nossa inspiração principal para a criação dessa aplicação foi o jogo "SUPER HOT" (https://superhotgame.com).
A divisão de tarefas foi a seguinte:
Gabriel:
- Estruturação inicial dos módulos e classes do projeto
- Funções de tratamento para desenho de objetos 3d no cenário
- Curvas de bézier
- Lógica dos inimigos
- Introdução e tratamento das colisões
- Lógica dos tiros e deleção de objetos 3d no cenário
- Adição da biblioteca de som
  
Pedro:
- Funcionamento da câmera
- Tratamento de callbacks do teclado e mouse
- Introdução e tratamento das texturas e iluminação
- Design e aplicação do mapa
- Lógica dos estados do jogo (menu, vitória, derrota)
- Câmera Look-at para o projétil

Sobre o uso da IA, tanto eu quanto minha dupla utilizamos com alguma frequência tanto o chatGPT quanto o Gemini quando tinhamos dificuldade em resolver algum problema do código, além do autocomplete do Copilot do Github. O Copilot, enquanto util para escrever secções de código rápidas e repetitivas, perdia-se muito no contexto ao sugerir qualquer função um pouco mais complexa, por outro lado, tanto o Gemini quanto o ChatGPT foram úteis em ajudar para resolver problemas específicos, mesmo que a montagem final e compreensão da solução proposta recaiam ainda sobre nós.

Critérios e porque eles estão implementados:
-Malhas poligonais complexas: Tanto o olho do final da fase quanto os Enemy.obj são mais complexos que cow.obj
-Transformações geométricas controladas pelo usuário: O usuário tem o controle direto e indireto de várias transformações geométricas, ao atirar ele cria um novo objeto que se move, ao trocar o tipo de câmera ele movimenta a arma para fora da tela, e ao acertar tiros em inimigos ele deleta os inimigos
-Câmera livre e câmera look-at: A câmera do jogador é uma câmera livre e existe a possibilidade de trocar a sua visão para uma câmera look-at para o último projétil disparado.
-Instâncias de objetos: Tanto paredes, quanto o teto e o chão são instâncias de um mesmo objeto cúbico, também os tiros e os inimigos são instâncias de um mesmo objeto.
-Três tipos de testes de intersecção: 
  - ponto-prisma, o mapa é dividido em 3 sub-áreas, é usado um teste de intesecção da posição do jogador com cada uma dessas sub-áreas para saber quais inimigos devem ser desenhados. Essa colisão também é usada para testar se o jogador consegue ver o inimigo, onde são testados vários pontos entre o inimigo e o jogador e, sob o caso de intersecção com algum prisma, o jogador não está sendo visto.
  - prisma-prisma é utilizado para detectar a colisão do jogador e dos inimigos com os objetos do mapa.
  - prisma-esfera é utilizado para testar se o tiro de um inimigo atingiu o jogador e vice-versa.
-Modelos de Iluminação Difusa e Blinn-Phong: enquanto as parede usam iluminação difusa, a arma, os inimigos e o olho final usam iluminação de blinn-phong
-Modelos de Interpolação de Phong e Gouraud: Quase todos os objetos desenhados na cena são interpolados com o modelo de phong, mas exclusivamente o projétil utiliza Gouraud.
-Mapeamento de texturas em todos os objetos: Quase todos os objetos possuem textura, o único que "não possui" uma textura dedicada é o inimigo, pois o estilo de arte do jogo no qual nos baseamos não dá texturas para os inimigos, que são coloridos de vermelho sólido, como é no nosso jogo.
-Movimentação com curva Bézier cúbica: O movimento da arma ao jogador andar é definido por uma curva de bézier cúbica
-Animações baseadas no tempo ($\Delta t$): Todas animações são baseadas no tempo, a única diferença é que elas são modificadas por um coeficiente controlado pela velocidade do jogador.
