/*
 * SistemaSolar.cpp - versão 2.4
 *
 * Copyright 2012 Rafael S. Andrade <rafaelsandrade@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 * Sistema Solar 2.4 - 29/12/2013 (Atualizado em 25/07/2014)
 *
 * Programa que simula o Sistema Solar
 *
 * Distâncias e comprimentos em UC. (Unidade de Comprimento)
 *                          1 UC = Raio da Terra em metros;
 * Massas em UM. (Unidade de Massa)
 *                          1 UM = Massa da Terra em kg;
 * Ângulos em graus;
 * Tempo em segundos;
 * Taxas de variação em graus/século;
 * Parâmetros dos planetas: http://ssd.jpl.nasa.gov;
 * Cálculos das posições dos planetas: http://ssd.jpl.nasa.gov/txt/aprx_pos_planets.pdf;
 * Parâmetros da Lua: Artigo "Numerical expressions for precession formulae and mean
 *                    elements for the Moon and the planets"
 *                    Endereço: http://adsabs.harvard.edu/abs/1994A%26A...282..663S;
 * Texturas: http://planetpixelemporium.com/index.php
 *
 * */

#include <GL/glut.h>
#include <time.h>
#include "Planeta.h"
#include "Interface.h"


CPlaneta *Planeta[MAX_PLANETA]; // Vetor dos planetas

float obsteta = 0.0;        // Angulo em rad de visualização do observador 'teta'
float obsfi = 1.5;          // Angulo em rad de visualização do observador 'fi'
float distancia = 10.0f;    // Distancia do observador ao planeta/objeto
IDPLANETA poscamera = TERRA;// Posição da câmera no Planeta/objeto
GLuint text_brilho_sol;     // Textura do brilho do sol
GLuint text_anel_saturno;   // Textura do anel de Saturno
GLuint text_anel_urano;     // Textura do anel de Urano
int bot_mouse = 0;          // Botão clicado do mouse (-1 se esquerdo e 1 se direito)

#define PASSO_TEMPO 5       // Passo de tempo

//////////////////////////////////////////////////////////////////////////
//                 Cria e inicializa os planetas                        //
//////////////////////////////////////////////////////////////////////////

void CriaPlaneta(void)
{
    printf("*** Sistema Solar - versao 2.4 (29/12/2013) ****");
    printf("\npor Rafael S. Andrade - rafaelsfg@hotmail.com\n\n");
    printf("Criando os planetas...\n");

    /////// Cria o Sol
    Planeta[SOL] = new CPlaneta;
    if(Planeta[SOL] == NULL)
    {
        printf("\n\nErro ao criar Sol: Falha na alocacao de memoria.\n\n");
        exit(1);
    }
    Planeta[SOL]->ParametroPlaneta("Sol", 109.044329538, 332918.214991, 0.0, EIXO_X, SOL);
    Planeta[SOL]->PeriodoRotacao(2192832.0);
    Planeta[SOL]->Textura("Texturas/sol.jpg");
    /// Carrega a textura do brilho do Sol
    glGenTextures ( 1, &text_brilho_sol );
    CarregarImagemJPEG("Texturas/brilho.jpg", text_brilho_sol);
    printf("\n'Sol' criado com sucesso.");

    // Ajusta o relógio para a hora atual
    Planeta[SOL]->AjustaRelogio(time(NULL));

    ////// Cria Mercúrio
    Planeta[MERCURIO] = Planeta[SOL]->AdicionaSatelite();
    Planeta[MERCURIO]->ParametroPlaneta("Mercurio", 0.382984801, 0.05527655, 0.035, EIXO_X, MERCURIO);
    Planeta[MERCURIO]->ParametroOrbita(9089.469468626, 48.33961819, 77.45771895, 0.20563661, 7.00559432, 252.25166724);
    Planeta[MERCURIO]->VariacaoParametroOrbita(0.0, -0.12214182, 0.15940013, 0.00002123, -0.00590158, 149472.67486623);
    Planeta[MERCURIO]->PeriodoRotacao(5067031.68);
    Planeta[MERCURIO]->AjustaRotacao(0.0f);
    Planeta[MERCURIO]->Textura("Texturas/mercurio.jpg");
    printf("\n'Mercurio' criado com sucesso.");

    //////// Cria Vênus
    Planeta[VENUS] = Planeta[SOL]->AdicionaSatelite();
    Planeta[VENUS]->ParametroPlaneta("Venus", 0.949896484, 0.815002678, 2.7, EIXO_X, VENUS); //177.3
    Planeta[VENUS]->ParametroOrbita(16984.32186177, 76.67261496, 131.76755713, 0.00676399, 3.39777545, 181.97970850);
    Planeta[VENUS]->VariacaoParametroOrbita(-0.006105068, -0.27274174, 0.05679648, -0.00005107, 0.00043494, 58517.81560260);
    Planeta[VENUS]->PeriodoRotacao(-20996798.4);
    Planeta[VENUS]->AjustaRotacao(-20.0f);
    Planeta[VENUS]->Textura("Texturas/venus.jpg");
    printf("\n'Venus' criado com sucesso.");

    /////// Cria a Terra
    Planeta[TERRA] = Planeta[SOL]->AdicionaSatelite();
    Planeta[TERRA]->ParametroPlaneta("Terra", 1.0, 1.0, -23.45, EIXO_X, TERRA);
    Planeta[TERRA]->ParametroOrbita(23481.033247102, 354.88739611, 102.93005885, 0.01673163, 0.00054346, 100.46691572);
    Planeta[TERRA]->VariacaoParametroOrbita(-0.000704431, -0.24123856, 0.31795260, -0.00003661, -0.01337178, 35999.37306329);
    Planeta[TERRA]->PeriodoRotacao(86163.084);
    Planeta[TERRA]->AjustaRotacao(123.0f);
    Planeta[TERRA]->Textura("Texturas/terra.jpg");
    printf("\n'Terra' criada com sucesso.");

    /////// Cria a Lua
    Planeta[TERRA_LUA] = Planeta[TERRA]->AdicionaSatelite();
    Planeta[TERRA_LUA]->ParametroPlaneta("Lua (Terra)", 0.272724419, 0.012302464, 6.67, EIXO_X, TERRA_LUA);
    Planeta[TERRA_LUA]->ParametroOrbita(60.17849471276924695, 125.04455504, 83.35324299, 0.055545526, 5.15668983, 218.31665436);
    Planeta[TERRA_LUA]->VariacaoParametroOrbita(5.964517399909904395E-7, -1935.53065616666666667, 4067.61673977777777778, 0.0000000016, 2.22222222222222222E-8, 481266.484371122222);
    Planeta[TERRA_LUA]->PeriodoRotacao(2360584.6848);
    Planeta[TERRA_LUA]->AjustaRotacao(105.0);
    Planeta[TERRA_LUA]->Textura("Texturas/lua.jpg");
    printf("\n'Lua' criada com sucesso.");

    /////// Cria Marte
    Planeta[MARTE] = Planeta[SOL]->AdicionaSatelite();
    Planeta[MARTE]->ParametroPlaneta("Marte", 0.53208204, 0.10744777, 25.19, EIXO_Y, MARTE);
    Planeta[MARTE]->ParametroOrbita(35778.335787752, 49.71320984, -23.91744784, 0.09336511, 1.85181869, -4.56813164);
    Planeta[MARTE]->VariacaoParametroOrbita(0.022776598, -0.26852431, 0.45223625, 0.00009149, -0.00724757, 19140.29934243);
    Planeta[MARTE]->PeriodoRotacao(88642.6632);
    Planeta[MARTE]->AjustaRotacao(210.0);
    Planeta[MARTE]->Textura("Texturas/marte.jpg");
    printf("\n'Marte' criado com sucesso.");

    /////// Cria Júpter
    Planeta[JUPITER] = Planeta[SOL]->AdicionaSatelite();
    Planeta[JUPITER]->ParametroPlaneta("Jupiter", 10.857462161, 317.7531137, 3.12, EIXO_X, JUPITER);
    Planeta[JUPITER]->ParametroOrbita(122159.588320052, 100.29282654, 14.27495244, 0.04853590, 1.29861416, 34.33479152);
    Planeta[JUPITER]->VariacaoParametroOrbita(-0.672496671, 0.13024619, 0.18199196, 0.00018026, -0.00322699, 3034.90371757);
    Planeta[JUPITER]->ParametroOrbitaAdicional(-0.00012452, 0.06064060, -0.35635438, 38.35125000);
    Planeta[JUPITER]->PeriodoRotacao(35729.685);
    Planeta[JUPITER]->AjustaRotacao(-225.0);
    Planeta[JUPITER]->Textura("Texturas/jupiter.jpg");
    printf("\n'Jupiter' criado com sucesso.");

    /////// Cria Saturno
    Planeta[SATURNO] = Planeta[SOL]->AdicionaSatelite();
    Planeta[SATURNO]->ParametroPlaneta("Saturno", 9.459724596, 95.13844248, -26.73, EIXO_X, SATURNO);
    Planeta[SATURNO]->ParametroOrbita(224044.210926453, 113.63998702, 92.86136063, 0.05550825, 2.49424102, 50.07571329);
    Planeta[SATURNO]->VariacaoParametroOrbita(-0.719693539, -0.25015002, 0.54179478, -0.00032044, 0.00451969, 1222.11494724);
    Planeta[SATURNO]->ParametroOrbitaAdicional(0.00025899, -0.13434469, 0.87320147, 38.35125000);
    Planeta[SATURNO]->PeriodoRotacao(38362.4);
    Planeta[SATURNO]->AjustaRotacao(0.0);
    Planeta[SATURNO]->Textura("Texturas/saturno.jpg");
    /// Carrega a textura do anel de Saturno
    glGenTextures ( 1, &text_anel_saturno);
    CarregarImagemJPEG("Texturas/saturno_anel.jpg", text_anel_saturno);
    printf("\n'Saturno' criado com sucesso.");

    /////// Cria Urano
    Planeta[URANO] = Planeta[SOL]->AdicionaSatelite();
    Planeta[URANO]->ParametroPlaneta("Urano", 4.011765795, 14.532325566, 97.86, EIXO_X, URANO);
    Planeta[URANO]->ParametroOrbita(450553.503014953, 73.96250215, 172.43404441, 0.04685740, 0.77298127, 314.20276625);
    Planeta[URANO]->VariacaoParametroOrbita(-4.803044486, 0.05739699, 0.09266985, -0.00001550, -0.00180155, 428.49512595);
    Planeta[URANO]->ParametroOrbitaAdicional(0.00058331, -0.97731848, 0.17689245, 7.67025000);
    Planeta[URANO]->PeriodoRotacao(-62064.0);
    Planeta[URANO]->AjustaRotacao(0.0);
    Planeta[URANO]->Textura("Texturas/urano.jpg");
    /// Carrega a textura do anel de Urano
    glGenTextures ( 1, &text_anel_urano);
    CarregarImagemJPEG("Texturas/urano_anel.jpg", text_anel_urano);
    printf("\n'Urano' criado com sucesso.");

    /////// Cria Netuno
    Planeta[NETUNO] = Planeta[SOL]->AdicionaSatelite();
    Planeta[NETUNO]->ParametroPlaneta("Netuno", 3.887295735, 17.143765903, 29.56, EIXO_XY, NETUNO);
    Planeta[NETUNO]->ParametroOrbita(706063.448330336, 131.78635853, 46.68158724, 0.00895439, 1.77005520, 304.22289287);
    Planeta[NETUNO]->VariacaoParametroOrbita(1.513821941, -0.00606302, 0.01009938, 0.00000818, 0.00022400, 218.46515314);
    Planeta[NETUNO]->ParametroOrbitaAdicional(-0.00041348, 0.68346318, -0.10162547, 7.67025000);
    Planeta[NETUNO]->PeriodoRotacao(57996.0);
    Planeta[NETUNO]->AjustaRotacao(0.0);
    Planeta[NETUNO]->Textura("Texturas/netuno.jpg");
    printf("\n'Netuno' criado com sucesso.");

    /////// Cria Plutão
    Planeta[PLUTAO] = Planeta[SOL]->AdicionaSatelite();
    Planeta[PLUTAO]->ParametroPlaneta("Plutao", 0.180662093, 0.002199679, 119.591, EIXO_X, PLUTAO);
    Planeta[PLUTAO]->ParametroOrbita(927192.113807427, 110.30167986, 224.09702598, 0.24885238, 17.14104260, 238.96535011);
    Planeta[PLUTAO]->VariacaoParametroOrbita(105.60616283, -0.00809981, -0.00968827, 0.00006016, 0.00000501, 145.18042903);
    Planeta[PLUTAO]->ParametroOrbitaAdicional(-0.01262724, 0.0, 0.0, 0.0);
    Planeta[PLUTAO]->PeriodoRotacao(-551854.08);
    Planeta[PLUTAO]->AjustaRotacao(0.0);
    Planeta[PLUTAO]->Textura("Texturas/plutao.jpg");
    printf("\n'Plutao' criado com sucesso.");
    printf("\n\nPlanetas criados com sucesso.\n");
}

//////////////////////////////////////////////////////////////////////////
//                       Desenha os objetos                             //
//////////////////////////////////////////////////////////////////////////

void DesenhaTela(void)
{
    double px, py, pz, obsx, obsy, obsz;
    GLfloat posicao_luz[]={0.0f,0.0f,0.0f,1.0f};

    // Recebe a posição do planeta que está observando
    Planeta[poscamera]->Posicao(&px, &py, &pz);

    // Calcula a posição da câmera
    obsx = px + distancia * sin(obsfi) * cos(obsteta);
    obsy = py + distancia * sin(obsfi) * sin(obsteta);
    obsz = pz + distancia * cos(obsfi);

    // Posiciona a câmera na órbita do planeta
    glLoadIdentity();
    gluLookAt(obsx, obsy, obsz, px, py, pz, 0.0f,0.0f,1.0f);

    // Limpa a tela
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Posição da Luz Solar
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    // Desenha os planetas
    Planeta[SOL]->DesenhaPlanetas();

    // Habilita a transparência e textura para o brilho so Sol e aneis dos planetas
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable ( GL_TEXTURE_2D );

    // Desenha o brilho do Sol
    glPushMatrix();
    glRotatef(obsteta * RAD_GRAU, 0.0, 0.0, 1.0);
    glRotatef(obsfi * RAD_GRAU, 0.0, 1.0, 0.0);
    glColor3f( 1.0f, 1.0f, 1.0f );
    float l = 1500.0 + norma(obsx, obsy, obsz) * 0.05;
    glBindTexture(GL_TEXTURE_2D, text_brilho_sol);
    Retangulo(l, l);
    glPopMatrix();

    // Desenha o anel de Saturno
    glPushMatrix();
    Planeta[SATURNO]->Posicao(&px, &py, &pz);
    glTranslatef(px, py, pz);
    glRotatef(-26.73, 1.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, text_anel_saturno);
    Anel(10.500689844, 22.002790766, 50);
    glPopMatrix();

    // Desenha o anel de Urano
    glPushMatrix();
    Planeta[URANO]->Posicao(&px, &py, &pz);
    glTranslatef(px, py, pz);
    glRotatef(-97.86, 1.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, text_anel_urano);
    Anel(6.566776696, 8.028397381, 50);
    glPopMatrix();

    // Desabilita a transparência e textura
    glDisable(GL_TEXTURE_2D);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    // Desenha as informações na tela
    InformacaoNaTela();

    glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
//               Função de iluminação do ambiente                       //
//////////////////////////////////////////////////////////////////////////

void IluminacaoAmbiente(void)
{
    GLfloat ambiente[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat difusa[] = { 20.0f, 20.0f, 20.0f, 1.0f };
    GLfloat especular[] = { 20.5f, 20.5f, 20.5f, 1.0f };

    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

//////////////////////////////////////////////////////////////////////////
//               Função para movimentar os planetas                     //
//////////////////////////////////////////////////////////////////////////

void Timer(int w)
{
    static clock_t t0 = clock();

    // Calcula as posições dos planetas
    Planeta[SOL]->MovimentoPlanetas();

    // Desenha os planetas
    glutPostRedisplay();

    // Calcula o tempo decorrido da simulação
    double dt = ((double)(clock() - t0)) / CLOCKS_PER_SEC;

    // Atualiza o relógio do Sistema Solar
    Planeta[SOL]->AtualizaRelogio(dt);

    t0 = clock();

    glutTimerFunc(PASSO_TEMPO, Timer, 1);
}

//////////////////////////////////////////////////////////////////////////
//               Redimensiona a Janela de visualização                  //
//////////////////////////////////////////////////////////////////////////

void AjusteDimensao(GLsizei w, GLsizei h)
{
    // Não deixa a largura da janela ser menor que 700
    if(w < 700) glutReshapeWindow(700, h);

    // Não deixa a altura da janela ser menor que 700
    if(h < 500) glutReshapeWindow(w, 500);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(23.902416667f, (GLfloat)w / (GLfloat)h, 0.5f, 9000000.0f);
    glMatrixMode(GL_MODELVIEW);
}

//////////////////////////////////////////////////////////////////////////
//              Gerencia eventos do botão do mouse                      //
//////////////////////////////////////////////////////////////////////////

void Mouse(int button, int state, int x, int y)
{
    if(state == GLUT_UP) /* Quando soltar o botão do mouse */
    {
        bot_mouse = 0;
        return;
    }

    switch(button)
    {
        case GLUT_LEFT_BUTTON: /* Clique com botão esquerdo */
            bot_mouse = -1;
        break;

        case GLUT_RIGHT_BUTTON: /* Clique com botão direito */
            bot_mouse = 1;
        break;
    }
}

/* Define a velocidade do mouse */
#define VEL_MOUSE 0.008

//////////////////////////////////////////////////////////////////////////
//  Função para mudar a visualização do espaço pelo movimento do mouse  //
//////////////////////////////////////////////////////////////////////////

void MoveMouse(int x, int y)
{
    float passo = VEL_MOUSE;
    float r = 0.0f, q = 0.0f;
    static int Xo = 0, Yo = 0;

    if(x > Xo) r = -passo;
    if(x < Xo) r = passo;
    if(y > Yo && obsfi > 0.02) q = -passo;
    if(y < Yo && obsfi < 3.13) q = passo;

    if(bot_mouse == -1)
    {
        obsteta += r;
        obsfi += q;
    }

    Xo = x;
    Yo = y;
}

//////////////////////////////////////////////////////////////////////////
//           Função para gerenciar eventos do Teclado                   //
//////////////////////////////////////////////////////////////////////////

void Teclado(unsigned char key, int a, int b)
{
    float t;
    time_t tempoatual;
    struct tm * timeinfo;
    static bool exibe_todas_orbitas = false;
    static bool exibe_todos_nomes = false;

    switch (key)
    {
        case 27:    // Fecha o programa
            exit(0);
            break;

        case '-':   // Diminue a distância até o planeta
            distancia -= (distancia - Planeta[poscamera]->Raio() - 1.09f) * 0.1f;
            break;

        case '+':   // Aumenta a distância até o planeta
            if(distancia < 4000000.0f) distancia *= 1.1f;
            break;

        case '.':   // Aumenta e velocidade do tempo
            t = Planeta[SOL]->VelocidadeTempo();
            if(fabs(t) < 135000.0) Planeta[SOL]->VelocidadeTempo(t * 2.0);
            if(fabs(t * 2.0 - 1.0) < 1.0E-5) Planeta[SOL]->VelocidadeTempo(1.0);
            break;

        case ',':   // Diminui a velocidade do tempo
            t = Planeta[SOL]->VelocidadeTempo();
            if(fabs(t) > 0.01) Planeta[SOL]->VelocidadeTempo(t / 2.0);
            if(fabs(t / 2.0 - 1.0) < 1.0E-5) Planeta[SOL]->VelocidadeTempo(1.0);
            break;

        case ' ':   // Pausa/Continua tempo
            if(Planeta[SOL]->VelocidadeTempo() == 0.0) Planeta[SOL]->VelocidadeTempo(1.0);
            else Planeta[SOL]->VelocidadeTempo(0.0);
            break;

        case ';':   // Inverte a direção do tempo
            Planeta[SOL]->VelocidadeTempo(-1.0 * Planeta[SOL]->VelocidadeTempo());
            break;

        case '/':   // Ajusta o relógio para a hora atual
            Planeta[SOL]->AjustaRelogio(time(NULL));
            Planeta[SOL]->VelocidadeTempo(1.0);
            break;

        case 'z':   // Diminue a escala de distâncias
            t = Planeta[SOL]->EscalaDistancia();
            if( t * 1.1 > 1.0 ) Planeta[SOL]->EscalaDistancia(1.0);
            else Planeta[SOL]->EscalaDistancia(t * 1.1);
            break;

        case 'x':   // Aumenta a escala de distâncias
            t = Planeta[SOL]->EscalaDistancia();
            if(fabs(t) > 0.02) Planeta[SOL]->EscalaDistancia(t * 0.9);
            break;

        case 'e':   // Exibe/Esconde os eixos
            Planeta[poscamera]->ExibirEixo(!Planeta[poscamera]->ExibirEixo());
            break;

        case 'r':   // Exibe/Esconde os meridianos e paralelos
            Planeta[poscamera]->ExibirMeridiano(!Planeta[poscamera]->ExibirMeridiano());
            break;

        case 'n':   // Exibe/Esconde o nome do planeta
            Planeta[poscamera]->ExibirNome(!Planeta[poscamera]->ExibirNome());
            break;

        case 'N':   // Exibir/Esconder todos os nomes dos planetas
            for(int i = 0; i < MAX_PLANETA; i++)
            {
                Planeta[i]->ExibirNome(!exibe_todos_nomes);
            }
            exibe_todos_nomes = !exibe_todos_nomes;
            break;

        case 'w':   // Exibe/Esconde plano órbita
            Planeta[poscamera]->ExibirPlanoOrbita(!Planeta[poscamera]->ExibirPlanoOrbita());
            break;

        case 'q':   // Exibe/Esconde plano equador
            Planeta[poscamera]->ExibirPlanoEquador(!Planeta[poscamera]->ExibirPlanoEquador());
            break;

        case 'o':   // Exibe/Esconde a órbita
            Planeta[poscamera]->ExibirOrbita(!Planeta[poscamera]->ExibirOrbita());
            Planeta[poscamera]->ExibirOrbitaAvancada(false);
            break;

        case 'O':   // Exibe/Esconde todas as órbitas dos planetas
            for(int i = 1; i < MAX_PLANETA; i++)
            {
                Planeta[i]->ExibirOrbita(!exibe_todas_orbitas);
                Planeta[i]->ExibirOrbitaAvancada(false);
            }
            exibe_todas_orbitas = !exibe_todas_orbitas;
            break;

        case 'p':   // Exibe/Esconde a órbita com o Periélio, Afélio e nós
            if(Planeta[poscamera]->ExibirOrbita())
                Planeta[poscamera]->ExibirOrbitaAvancada(!Planeta[poscamera]->ExibirOrbitaAvancada());
            break;

        case 'l':   // Exibe/Esconde a linha até o planeta pai
            Planeta[poscamera]->ExibirLinhaPai(!Planeta[poscamera]->ExibirLinhaPai());
            break;

        case 'i':   // Exibe/Esconde informações extras
            exibe_info_extra = !exibe_info_extra;
            break;

        case 'a':   // Almenta 1 dia no calendário
            tempoatual = Planeta[SOL]->Relogio();
            timeinfo = localtime ( &tempoatual );
            timeinfo->tm_mday = timeinfo->tm_mday + 1;
            if(timeinfo->tm_year < 136) Planeta[SOL]->AjustaRelogio(mktime(timeinfo));
            break;

        case 'A':   // Diminue 1 dia no calendário
            tempoatual = Planeta[SOL]->Relogio();
            timeinfo = localtime ( &tempoatual );
            timeinfo->tm_mday = timeinfo->tm_mday - 1;
            if(timeinfo->tm_year > 1) Planeta[SOL]->AjustaRelogio(mktime(timeinfo));
            break;

        case 's':   // Aumenta 1 mês no calendário
            tempoatual = Planeta[SOL]->Relogio();
            timeinfo = localtime ( &tempoatual );
            timeinfo->tm_mon = timeinfo->tm_mon + 1;
            if(timeinfo->tm_year < 136) Planeta[SOL]->AjustaRelogio(mktime(timeinfo));
            break;

        case 'S':   // Diminue 1 mês no calendário
            tempoatual = Planeta[SOL]->Relogio();
            timeinfo = localtime ( &tempoatual );
            timeinfo->tm_mon = timeinfo->tm_mon - 1;
            if(timeinfo->tm_year > 1) Planeta[SOL]->AjustaRelogio(mktime(timeinfo));
            break;

        case 'd':   // Aumenta 1 ano no calendário
            tempoatual = Planeta[SOL]->Relogio();
            timeinfo = localtime ( &tempoatual );
            timeinfo->tm_year = timeinfo->tm_year + 1;
            if(timeinfo->tm_year < 136) Planeta[SOL]->AjustaRelogio(mktime(timeinfo));
            break;

        case 'D':   // Diminue 1 ano no calendário
            tempoatual = Planeta[SOL]->Relogio();
            timeinfo = localtime ( &tempoatual );
            timeinfo->tm_year = timeinfo->tm_year - 1;
            if(timeinfo->tm_year > 1) Planeta[SOL]->AjustaRelogio(mktime(timeinfo));
            break;

        case '0':   // Posiciona a câmera no Sol
            poscamera = SOL;
            distancia = 2000.0f;
            break;

        case '1':   // Posiciona a câmera em Mercúrio
            poscamera = MERCURIO;
            distancia = 4.0f;
            break;

        case '2':   // Posiciona a câmera em Venus
            poscamera = VENUS;
            distancia = 10.0f;
            break;

        case '3':   // Posiciona a câmera na Terra
            poscamera = TERRA;
            distancia = 10.0f;
            break;

        case '4':   // Posiciona a câmera em Marte
            poscamera = MARTE;
            distancia = 5.0f;
            break;

        case '5':   // Posiciona a câmera em Júpiter
            poscamera = JUPITER;
            distancia = 70.0f;
            break;

        case '6':   // Posiciona a câmera em Saturno
            poscamera = SATURNO;
            distancia = 100.0f;
            break;

        case '7':   // Posiciona a câmera em Urano
            poscamera = URANO;
            distancia = 70.0f;
            break;

        case '8':   // Posiciona a câmera em Netuno
            poscamera = NETUNO;
            distancia = 70.0f;
            break;

        case '9':   // Posiciona a câmera em Plutão
            poscamera = PLUTAO;
            distancia = 3.0f;
            break;

        case '#':   // Posiciona a câmera na Lua
            poscamera = TERRA_LUA;
            distancia = 3.0f;
            break;
    }
}

//////////////////////////////////////////////////////////////////////////
//                      Programa Principal                              //
//////////////////////////////////////////////////////////////////////////

int main(int argc, char*argv[])
{
    glutInit (&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Sistema Solar - 2.4 (por Rafael Andrade)");
    glutDisplayFunc(DesenhaTela);
    glutReshapeFunc(AjusteDimensao);
    glutMouseFunc(Mouse);
    glutMotionFunc(MoveMouse);
    glutKeyboardFunc(Teclado);
    IluminacaoAmbiente();
    CriaPlaneta();
    Timer(1);
    glutMainLoop();
    return 0;
}
