# Simulador do Sistema Solar
Este programa simula em tempo real as posições e os movimentos de translação e rotação dos oito planetas do Sistema Solar, do Sol, da Lua e de Plutão, permitindo ao usuário avançar, regredir e parar o tempo da simulação, visualizar as linhas das órbitas e obter informações sobre o corpo celeste desejado.

Trata-se de um programa experimental, mas que permite sua utilização para fins educacionais, podendo demonstrar, por exemplo, o movimento e velocidade de translação dos planetas, como o movimento de rotação da Terra determina os dias e as noites, como sua inclinação influencia o clima, eclipses solares e lunares, dentre outros.

O código fonte do programa está escrito na linguagem C++, e utiliza a biblioteca Opengl juntamente com a glut.h para criar a interface gráfica. Para carregar as texturas dos corpos, que estão em formato jpeg, utilizamos a biblioteca libjpeg.

Para simplificar a simulação, todos os corpos celestes são desenhados como esferas perfeitas, o que não reflete à realidade. 

Como os raios dos corpos e as distâncias entre eles são muito grandes, definimos uma escala chamada de Unidade de Comprimento (U.C.) onde 1 U.C. corresponde ao raio da Terra em metros. Assim evitamos que o programa trabalhe com números grandes, como por exemplo, o raio do Sol que mede aproximadamente 695.800.000 metros, equivalente a 109,04 U.C.

Para as massas dos corpos foi adotado o mesmo critério acima, definindo a Unidade de Massa (U.M.) que é igual à massa da Terra em quilogramas.

No canto superior esquerdo da tela do programa temos as informações do corpo celeste selecionado: seu nome, raio médio, massa, distância entre o observador e o corpo, velocidade orbital e distância entre o corpo observado e o corpo que ele orbita (no caso do Sol estas duas últimas não são exibidas).

No canto superior direito temos a data e a hora da simulação, que podem ser alteradas com os comandos do teclado, permitindo avançar, retroceder e pausar o tempo, aumentar ou diminuir a velocidade da simulação, ou ir para uma data específica.

A parte inferior esquerda mostra a escala de distância, onde "1:1" significa que as distâncias estão nas proporções reais. Através do teclado podemos alterar esta escala, tornando os planetas mais próximos uns dos outros.

A visualização é centrada no corpo celeste desejado, selecionado através das teclas numéricas de 0 a 9. Movimentando o mouse com o botão esquerdo pressionado, podemos girar em torno do corpo celeste.

As variações dos elementos orbitais implementadas no programa não são suficientes para representar com precisão as posições reais dos corpos celestes, devido a complexa interação gravitacional entre eles.

Portanto não é recomendado o uso deste simulador para aplicações que requerem muita precisão. Existem soluções mais precisas como o sistema Horizons da JPL/Nasa e VSOP2013.

Na simulação, a velocidade do movimento de rotação dos corpos é constante, o que na verdade não ocorre. A velocidade de rotação da Terra, por exemplo, está diminuindo cada vez mais, tornando os dias e as noites terrestres mais longos com o passar do tempo.
