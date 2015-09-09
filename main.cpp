/**
 * SistemaSolar_3.cpp
 *
 * 08/02/2015
 *
 * Copyright 2015 Rafael Andrade
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
 * Simulador do Sistema Solar - versão 3.0
 */

#include <time.h>
#include "CorpoCeleste.h"

// Ajustes
#define MAX_VIEW  9000000.0 // Distância máxima de visualização
#define MIN_VIEW  0.5	      // Distância mínima de visualização
#define VEL_GIRO  0.0005	  // Velocidade do giro da câmera
#define VEL_CAM   0.005      // Velocidade de mudança da posição lateral da câmera
#define VEL_DIST  0.05      // Velocidade de mudança da distância do foco à câmera

float camFi = 0.2, camTeta = 0.0f;	//  Angulos da posição da câmera
float distCamFoco = 3000.0;  // Distância entre a câmera e o foco
float posCam[3];	// Posição da câmera
double posFoco[3];  // Posição do foco da câmera
int botMouse;		// Indica qual botão do mouse foi pressionado
unsigned int dT;	// Diferença de tempo entre os frames
const GLfloat posLuz[] = {0.0f, 0.0f, 0.0f, 1.0f}; // Posição da luz do Sol

// Enumeração dos corpos celestes
enum ID_CORPO {SOL, MERCURIO, VENUS, TERRA, LUA_TERRA, MARTE, JUPITER, MAX_CORPO};

ID_CORPO selCorpo = TERRA;  // Corpo selecionado para exibir informações na tela

// Lista dos corpos celestes
CorpoCeleste listaCorpo[MAX_CORPO];


/** ////////////////////////////////////////////////////////////////////////
//
//	Função para inicializar os corpos celestes
//
///////////////////////////////////////////////////////////////////////// */

void initCorposCelestes()
{
    // Ajusta o relógio
    CorpoCeleste::ajustaRelogio(time(NULL));

    // Ajusta as escalas
    CorpoCeleste::setEscalaOrbita(0.03);
    CorpoCeleste::setEscalaRaio(45.0);

    // Cria o Sol
    listaCorpo[SOL].dadosCorpo("Dados/Sol.txt", SOL);
    listaCorpo[SOL].textura("Texturas/sol.jpg");
	//listaCorpo[SOL].setExibeEixos(true);

    // Cria Mercúrio
    listaCorpo[MERCURIO].dadosCorpo("Dados/Mercurio.txt", MERCURIO);
    listaCorpo[MERCURIO].textura("Texturas/mercurio.jpg");
    listaCorpo[MERCURIO].setCorpoPai(&listaCorpo[SOL]);

    // Cria Vênus
    listaCorpo[VENUS].dadosCorpo("Dados/Venus.txt", VENUS);
    listaCorpo[VENUS].textura("Texturas/venus.jpg");
	listaCorpo[VENUS].setCorpoPai(&listaCorpo[SOL]);
	listaCorpo[VENUS].setDiffRot(190.0);

	// Cria a Terra
	listaCorpo[TERRA].dadosCorpo("Dados/Terra.txt", TERRA);
	listaCorpo[TERRA].textura("Texturas/terra.jpg");
	listaCorpo[TERRA].setDiffRot(192.0);
	listaCorpo[TERRA].setCorpoPai(&listaCorpo[SOL]);

	// Cria a Lua da Terra
	listaCorpo[LUA_TERRA].dadosCorpo("Dados/Lua_Terra.txt", LUA_TERRA);
	listaCorpo[LUA_TERRA].textura("Texturas/lua.jpg");
	listaCorpo[LUA_TERRA].setDiffRot(95.0);
	listaCorpo[LUA_TERRA].setExibeNome(false);
	listaCorpo[LUA_TERRA].setCorpoPai(&listaCorpo[TERRA]);

	// Cria Marte
	listaCorpo[MARTE].dadosCorpo("Dados/Marte.txt", MARTE);
	listaCorpo[MARTE].textura("Texturas/marte.jpg");
	listaCorpo[MARTE].setCorpoPai(&listaCorpo[SOL]);
    listaCorpo[MARTE].setDiffRot(205.0);

    // Cria Júpiter
	listaCorpo[JUPITER].dadosCorpo("Dados/Jupiter.txt", JUPITER);
	listaCorpo[JUPITER].textura("Texturas/jupiter.jpg");
	listaCorpo[JUPITER].setCorpoPai(&listaCorpo[SOL]);
    //listaCorpo[JUPITER].setDiffRot(205.0);
}


/** ///////////////////////////////////////////////////////////////////////
//
//  Função para desenhar textos
//
//////////////////////////////////////////////////////////////////////// */

void desenhaTexto(float x, float y, float z, void *fonte, const char *string)
{
    glDisable(GL_LIGHTING);
	glRasterPos3f(x, y, z);
	while(*string) glutBitmapCharacter(fonte, *string++);
	glEnable(GL_LIGHTING);
}


/** ///////////////////////////////////////////////////////////////////////
//
//  Função para exibir informações na tela
//
//////////////////////////////////////////////////////////////////////// */

void informacaoNaTela(void)
{
	char buffer[80];

	// Muda a tela para 2D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
    glDisable(GL_LIGHTING);

	// Desenha o relogio
	time_t relogio = CorpoCeleste::getRelogio();
	struct tm * timeinfo = localtime ( &relogio );
	strftime (buffer, 80, "%d/%m/%Y - %H:%M:%S", timeinfo);
	glColor3f(1.0f, 1.0f, 1.0f);
	desenhaTexto(glutGet(GLUT_WINDOW_WIDTH) - 200, 30, 0, GLUT_BITMAP_HELVETICA_18, buffer);

	// Desenha a velocidade do tempo
	float vel = CorpoCeleste::getVelTempo();

	if(vel == 0.0)
        sprintf(buffer, "Tempo parado");
	else
	{
		if(fabs(vel) > 1.0)
            sprintf(buffer, "%gx mais depressa", vel);
		else
		{
			if(fabs(vel) < 1.0)
                sprintf(buffer, "%gx mais devagar", 1.0f/vel);
			else
			{
				if(vel == 1.0)
                    sprintf(buffer, "Tempo real");
				else
					sprintf(buffer, "Tempo inverso");
			}
		}
	}
	desenhaTexto(glutGet(GLUT_WINDOW_WIDTH) - 200, 50, 0, GLUT_BITMAP_HELVETICA_12, buffer);

	///Informações do planeta

	//Nome
	glColor3f(1.0f, 0.0f, 0.0f);
	desenhaTexto(20.0, 30.0, 0.0, GLUT_BITMAP_TIMES_ROMAN_24, listaCorpo[selCorpo].getNome());

	//Raio
	glColor3f(1.0f, 0.5f, 0.0f);
	sprintf(buffer, "Raio medio: %G km", listaCorpo[selCorpo].getRaio() * R_TERRA);
	desenhaTexto(20.0, 60.0, 0.0, GLUT_BITMAP_HELVETICA_12, buffer);

	// Massa
	sprintf(buffer, "Massa: %G kg", listaCorpo[selCorpo].getMassa() * M_TERRA);
	desenhaTexto(20.0, 75.0, 0.0, GLUT_BITMAP_HELVETICA_12, buffer);

    if(selCorpo != SOL)
	{
	    // Velocidade orbital
		sprintf(buffer, "Velocidade orbital: %g km/s", listaCorpo[selCorpo].getVelOrbital() * R_TERRA);
		desenhaTexto(20, 90, 0, GLUT_BITMAP_HELVETICA_12, buffer);

		// Distância do pai
		double d = listaCorpo[selCorpo].getDistanciaPai() * R_TERRA;
		sprintf(buffer, "Distancia (%s): %.1f km (%.4f UA)", listaCorpo[selCorpo].getNomePai(), d, d / CONST_UA);
		desenhaTexto(20, 105, 0, GLUT_BITMAP_HELVETICA_12, buffer);
    }

	/* // Exibe informações extras
	if(exibe_info_extra && poscamera != SOL)
	{
		sprintf(buffer, "Elementos Orbitais medios:");
		DesenhaTexto(20, 135, 0, GLUT_BITMAP_HELVETICA_12, buffer);
		double se = Planeta[poscamera]->SemiEixo();
		sprintf(buffer, "Semi eixo: %.1f km (%.8g UA)", se*R_TERRA_KM, se * R_TERRA_KM / UA_KM);
		DesenhaTexto(30, 150, 0, GLUT_BITMAP_HELVETICA_12, buffer);
		sprintf(buffer, "No ascendente: %.8g graus", Planeta[poscamera]->NoAscendente());
		DesenhaTexto(30, 165, 0, GLUT_BITMAP_HELVETICA_12, buffer);
		sprintf(buffer, "Arg. Perielio: %.8g graus", Planeta[poscamera]->ArgumentoPerielio());
		DesenhaTexto(30, 180, 0, GLUT_BITMAP_HELVETICA_12, buffer);
		sprintf(buffer, "Excentricidade: %.8g", Planeta[poscamera]->Excentricidade());
		DesenhaTexto(30, 195, 0, GLUT_BITMAP_HELVETICA_12, buffer);
		sprintf(buffer, "Anomalia Media: %.8g graus", Planeta[poscamera]->AnomaliaMedia());
		DesenhaTexto(30, 210, 0, GLUT_BITMAP_HELVETICA_12, buffer);
		sprintf(buffer, "Inclinacao Orbital: %.8g graus", Planeta[poscamera]->InclinacaoOrbital());
		DesenhaTexto(30, 225, 0, GLUT_BITMAP_HELVETICA_12, buffer);
		float p = Planeta[poscamera]->PeriodoTranslacao();
		sprintf(buffer, "Periodo Translacao: %.8g dias (%g anos)", p / 86400.0, p / 31557600.0);
		DesenhaTexto(20, 240, 0, GLUT_BITMAP_HELVETICA_12, buffer);
		p = Planeta[poscamera]->PeriodoRotacao();
		sprintf(buffer, "Periodo Rotacao: %.8g horas (%g dias)", p / 3600.0, p / 86400.0);
		DesenhaTexto(20, 255, 0, GLUT_BITMAP_HELVETICA_12, buffer);
	}
*/
	// Escala da distância da órbita
	sprintf(buffer, "Escala de distancia: 1:%G", 1.0 / CorpoCeleste::getEscalaOrbita());
	desenhaTexto(20, glutGet(GLUT_WINDOW_HEIGHT) - 25, 0, GLUT_BITMAP_HELVETICA_12, buffer);

    // Escala dos raios dos corpos (exceto o Sol)
    sprintf(buffer, "Escala do raio: 1:%G", CorpoCeleste::getEscalaRaio());
	desenhaTexto(20, glutGet(GLUT_WINDOW_HEIGHT) - 10, 0, GLUT_BITMAP_HELVETICA_12, buffer);

	// Volta a tela para 3D
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
}

/** ///////////////////////////////////////////////////////////////////////
//
//	Função para desenhar os elementos na tela
//
//////////////////////////////////////////////////////////////////////// */

void desenhaTela()
{
	glLoadIdentity();

    // Posiciona a Câmera
	gluLookAt(posCam[0], posCam[1], posCam[2],
			  posFoco[0], posFoco[1], posFoco[2], 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Posiciona a luz
	glLightfv(GL_LIGHT0, GL_POSITION, posLuz);

	////////////////////// Área do Desenho /////////////////////////////

    // Mostra/esconde o nome da Lua dependendo da distância e da escala
    if(distCamFoco < 400 * CorpoCeleste::getEscalaOrbita())
        listaCorpo[LUA_TERRA].setExibeNome(true);
    else
        listaCorpo[LUA_TERRA].setExibeNome(false);

    // Desenha os corpos
	for(int i = 0; i < MAX_CORPO; i++)
	{
		listaCorpo[i].desenha();
	}

    // Exibe informações na tela
    informacaoNaTela();
	////////////////////////////////////////////////////////////

	glutSwapBuffers();
}

/** ///////////////////////////////////////////////////////////////////////
//
//  Função de Iluminação do Ambiente
//
//////////////////////////////////////////////////////////////////////// */

void iluminacao()
{
    const GLfloat ambiente[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
    const GLfloat difusa[]	  = { 20.0f, 20.0f, 20.0f, 20.0f };
	const GLfloat especular[] = { 20.0f, 20.0f, 20.0f, 20.0f };

	const GLfloat matAmbiente[]   = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat matDifusa[]     = { 0.8f, 0.8f, 0.8f, 1.0f };
	const GLfloat matEspecular[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
	const GLfloat highShininess[] = { 100.0f };

    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);
	glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular);

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDifusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matEspecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, highShininess);
}

/** ///////////////////////////////////////////////////////////////////////
//
//  Função de animação da Simulação
//
//////////////////////////////////////////////////////////////////////// */

void simulacao()
{
	// Calcula a diferença de tempo entre os frames
	static unsigned int t0 = glutGet(GLUT_ELAPSED_TIME);
	unsigned int t1 = glutGet(GLUT_ELAPSED_TIME);

	dT = t1 - t0;

	t0 = t1;
	/////////

    // Atualizao o relógio
    CorpoCeleste::atualizaRelogio((double)dT / 1000.0);

    // Atualiza os corpos
    for(int i = 0; i < MAX_CORPO; i++)
	{
        listaCorpo[i].atualizaCorpo();
	}

	glutPostRedisplay();	// Redesenha a tela
}

/** ///////////////////////////////////////////////////////////////////////
//
//  Função usada para especificar o volume de visualização
//
//////////////////////////////////////////////////////////////////////// */

void ajusteJanela(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(24.0, (GLdouble)w / (GLdouble)h, MIN_VIEW, MAX_VIEW);
	glMatrixMode(GL_MODELVIEW);

	// Calcula a posição inicial da câmera
	listaCorpo[TERRA].getPosicao(posFoco);
    posCam[0] = posFoco[0] + distCamFoco * cos(camFi) * cos(camTeta);
    posCam[1] = posFoco[1] + distCamFoco * cos(camFi) * sin(camTeta);
    posCam[2] = posFoco[2] + distCamFoco * sin(camFi);
}

/** ///////////////////////////////////////////////////////////////////////
//
//  Função para gerenciar os botões do mouse
//
//////////////////////////////////////////////////////////////////////// */

void botaoMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
		botMouse = button;
}

/** ///////////////////////////////////////////////////////////////////////
//
//  Função para mudar a visualização do espaço pelo movimento do mouse
//
//////////////////////////////////////////////////////////////////////// */

void moveMouse(int x, int y)
{
	float velGiro = VEL_GIRO * dT;
	float velCam = VEL_CAM * distCamFoco * dT;
	float r = 0.0f, q = 0.0f;
	static int x0 = 0, y0 = 0;

    // Movimento horizontal do mouse
	if(x > x0)
		r = -velGiro;
	else
        if(x < x0)
			r = velGiro;

    // Movimento vertical do mouse
	if(y > y0 && (camFi + velGiro) < M_PI_2)
		q = velGiro;
	else
        if(y < y0 && (camFi - velGiro) > -M_PI_2)
			q = -velGiro;

    // Botão esquerdo do mouse pressionado - gira a câmera
	if(botMouse == GLUT_LEFT_BUTTON)
	{
		camTeta += r;
		camFi += q;
	}
	else
        if(botMouse == GLUT_RIGHT_BUTTON)  // Boão direito do mouse pressionado -
        {                                  // muda a posição do foco da câmera
            posFoco[0] += -velCam * r * sin(camTeta);
			posFoco[1] += velCam * r * cos(camTeta);
			posFoco[2] += velCam * q;
        }

	// Atualiza posição da câmera
    posCam[0] = posFoco[0] + distCamFoco * cos(camFi) * cos(camTeta);
    posCam[1] = posFoco[1] + distCamFoco * cos(camFi) * sin(camTeta);
    posCam[2] = posFoco[2] + distCamFoco * sin(camFi);

	x0 = x;
	y0 = y;
}

/** ///////////////////////////////////////////////////////////////////////
//
//  Função para gerenciar as teclas especiais do teclado
//
//////////////////////////////////////////////////////////////////////// */

void tecladoEspecial(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT);
    if(key == GLUT_KEY_RIGHT);
    if(key == GLUT_KEY_UP);
    if(key == GLUT_KEY_DOWN);
}


/** ///////////////////////////////////////////////////////////////////////
//
//  Função para focalizar a câmera no corpo celeste
//
//////////////////////////////////////////////////////////////////////// */

void focalizaCamera(enum ID_CORPO corpo)
{
    listaCorpo[corpo].getPosicao(posFoco);
    selCorpo = corpo;

    // Mantém a distância da câmera maior que o raio do corpo selecionado
    if(corpo != SOL)
    {
        if(distCamFoco < (listaCorpo[corpo].getRaio() * CorpoCeleste::getEscalaRaio() * 10.0))
            distCamFoco = listaCorpo[corpo].getRaio() * CorpoCeleste::getEscalaRaio() * 10.0;
    }
    else
    {
        if(distCamFoco < 1000.0)
            distCamFoco = 1000.0;
    }

    // Posiciona a câmera na podição do corpo selecionado
    posCam[0] = posFoco[0] + distCamFoco * cos(camFi) * cos(camTeta);
    posCam[1] = posFoco[1] + distCamFoco * cos(camFi) * sin(camTeta);
    posCam[2] = posFoco[2] + distCamFoco * sin(camFi);
}



/** ///////////////////////////////////////////////////////////////////////
//
//  Função para gerenciar o teclado
//
//////////////////////////////////////////////////////////////////////// */

void teclado(unsigned char key, int a, int b)
{
    float temp;

    switch (key)
    {
		case 27:    // Fecha o programa
			exit(0);
			break;

        case '0':   // Focaliza a câmera no Sol
            focalizaCamera(SOL);
            break;

        case '1':   // Focaliza a câmera no planeta Mercúrio
            focalizaCamera(MERCURIO);
            break;

        case '2':   // Focaliza a câmera no planeta Vênus
            focalizaCamera(VENUS);
            break;

        case '3':   // Focaliza a câmera no planeta Terra
            focalizaCamera(TERRA);
            break;

        case '#':   // Focaliza a câmera na Lua da Terra
            focalizaCamera(LUA_TERRA);
            break;

        case '4':   // Focaliza a câmera no planeta Marte
            focalizaCamera(MARTE);
            break;

        case '5':   // Focaliza a câmera no planeta Júpiter
            focalizaCamera(JUPITER);
            break;

        case '+':   // Aumenta a distãncia entre o foco e a câmera
        case '=':
            distCamFoco += VEL_DIST * distCamFoco;

            if(distCamFoco < MIN_VIEW)  // Limita a distância da vizualização mínima
                distCamFoco = MIN_VIEW + VEL_DIST * distCamFoco;

            posCam[0] = posFoco[0] + distCamFoco * cos(camFi) * cos(camTeta);
            posCam[1] = posFoco[1] + distCamFoco * cos(camFi) * sin(camTeta);
            posCam[2] = posFoco[2] + distCamFoco * sin(camFi);
            break;

        case '-':   // Diminue a distância entre o foco e a câmera
            distCamFoco -= VEL_DIST * distCamFoco;

            if(distCamFoco > MAX_VIEW) // Limita a distância da vizualização máxima
                distCamFoco = MAX_VIEW - VEL_DIST * distCamFoco;

            posCam[0] = posFoco[0] + distCamFoco * cos(camFi) * cos(camTeta);
            posCam[1] = posFoco[1] + distCamFoco * cos(camFi) * sin(camTeta);
            posCam[2] = posFoco[2] + distCamFoco * sin(camFi);
            break;

        case 'x':   // Aumenta a escala do tamanho da órbita
            CorpoCeleste::setEscalaOrbita(CorpoCeleste::getEscalaOrbita() * 1.1);

            if(CorpoCeleste::getEscalaOrbita() > 1.0) // Limita a escala para 1:1
                CorpoCeleste::setEscalaOrbita(1.0);

            break;

        case 'c':   // Diminue a escala do raio dos corpos (exceto o Sol)
            CorpoCeleste::setEscalaRaio(CorpoCeleste::getEscalaRaio() * 0.9);

            if(fabs(CorpoCeleste::getEscalaRaio() - 1.0) < 0.11)
                CorpoCeleste::setEscalaRaio(1.0);
            break;

        case 'v':   // Aumenta a escala do raio dos corpos (exceto o Sol)
            if(CorpoCeleste::getEscalaRaio() < 100.0)
                CorpoCeleste::setEscalaRaio(CorpoCeleste::getEscalaRaio() * 1.1);

            break;

        case 'z':   // Diminui a escala do tamanho da órbita
            CorpoCeleste::setEscalaOrbita(CorpoCeleste::getEscalaOrbita() * 0.9);
            break;

        case '.':   // Aumenta a velocidade do tempo
            temp = CorpoCeleste::getVelTempo();
			if(fabs(temp) < 135000.0)
                CorpoCeleste::setVelTempo(temp * 2.0);
			if(fabs(temp * 2.0 - 1.0) < 1.0E-5)
                CorpoCeleste::setVelTempo(1.0);
            break;

        case ',':   // Diminui a velocidade do tempo
            temp = CorpoCeleste::getVelTempo();
			if(fabs(temp) > 0.01)
                CorpoCeleste::setVelTempo(temp / 2.0);
			if(fabs(temp / 2.0 - 1.0) < 1.0E-5)
                CorpoCeleste::setVelTempo(1.0);
            break;

        case ' ':   // Para o tempo
            if(CorpoCeleste::getVelTempo() == 0.0)
                CorpoCeleste::setVelTempo(1.0);
            else
                CorpoCeleste::setVelTempo(0.0);
            break;

        case '/':   // Ajusta o tempo para o atual
            CorpoCeleste::ajustaRelogio(time(NULL));
            CorpoCeleste::setVelTempo(1.0);
            break;

        case ';':   // Inverte a direção do tempo
            CorpoCeleste::setVelTempo(-CorpoCeleste::getVelTempo());
            break;
    }
}


/** ///////////////////////////////////////////////////////////////////////
//
//  Função principal
//
//////////////////////////////////////////////////////////////////////// */

int main(int argc, char *argv[])
{
    glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(150, 50);
	glutCreateWindow("Sistema Solar - v. 3.0");
	glutDisplayFunc(desenhaTela);
	glutReshapeFunc(ajusteJanela);
	glutMouseFunc(botaoMouse);
	glutMotionFunc(moveMouse);
	glutSpecialFunc(tecladoEspecial);
	glutKeyboardFunc(teclado);
	glutIdleFunc(simulacao);
	iluminacao();
	initCorposCelestes();
	glutMainLoop();
	return 0;
}
