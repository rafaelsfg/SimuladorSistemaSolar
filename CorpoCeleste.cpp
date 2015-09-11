/**
 * CorpoCeleste.cpp
 *
 * 12/02/2015
 *
 * Copyright 2015 Rafael Andrade <rafaelsandrade@gmail.com>
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
 * Implementação das funções da classe "CorpoCeleste"
 */

#include "CorpoCeleste.h"

double CorpoCeleste::relogio;   // Relógio do Sistema Solar
float CorpoCeleste::escalaOrbita;  // Escala do tamanho das órbitas
float CorpoCeleste::escalaRaio;    // Escala do tamanho do raio dos corpos (exceto o Sol)
float CorpoCeleste::velTempo;  // Velocidade do tempo

/*******************************************************************\
|                          Ajustes                                  |
\*******************************************************************/

// Carrega de um arquivo os parâmetros físicos e orbitais do corpo
void CorpoCeleste::dadosCorpo(const char *nome_arq, unsigned int id_Corpo)
{
    FILE *arq;

    arq = fopen(nome_arq, "r");

    if(arq == NULL)
    {
        perror("\n\nArquivo nao encontrado");
        exit(1);
    }

    ///////// Dados físicos

    // Nome
    fgets(nomeCorpo, 20, arq);

    // Coloca o '\0' no final do nome
    int i = 18;
    while(nomeCorpo[i--] == ' ');

    nomeCorpo[i + 2] = '\0';

    // Lê os demais dados físicos
    fscanf(arq, "%f %lf %f %f %lf %d", &raioCorpo, &massaCorpo, &inclEixoX,
                                       &inclEixoY, &periodoRot, &ehSol);

    // ID do Corpo
    idCorpo = id_Corpo;

    // Se o corpo for Sol não carrega os elementos orbitais
    // O programa considera o Sol fixo no espaço
    if(ehSol == true)
    {
        fclose(arq);
        return;
    }

    ///////// Dados Orbitais

    // Semi-eixo maior
    fscanf(arq, "%lf %lf", &SE_0, &varSE);

    // Excentricidade
    fscanf(arq, "%lf %lf", &EX_0, &varEX);

    // Inclinação Orbital
    fscanf(arq, "%lf %lf", &IO_0, &varIO);

    // Longitude Média
    fscanf(arq, "%lf %lf", &LM_0, &varLM);

    // Longitude do Periélio
    fscanf(arq, "%lf %lf", &LP_0, &varLP);

    // Longitude do Nó Ascendente
    fscanf(arq, "%lf %lf", &NA_0, &varNA);

    fclose(arq);
}

// Adiciona a Textura do corpo
void CorpoCeleste::textura(const char *nome_textura)
{
    glGenTextures(1, &idTextura);
	carregarImagemJPEG(nome_textura, idTextura);
}


/*******************************************************************\
|                            Ações                                  |
\*******************************************************************/


// Desenha o corpo
void CorpoCeleste::desenha()
{
    double posPai[3];

    glPushMatrix();

    // Translada para o centro da órbita
    if(!ehSol)
    {
        corpoPai->getPosicao(posPai);
        glTranslatef(posPai[0], posPai[1], posPai[2]);
    }

    // Desenha a órbita do corpo
    if(!ehSol && exibeOrbita)
        desenhaOrbita();

    // Translada o corpo para a posição na órbita
    glTranslatef(posX, posY, posZ);

    // Exibe o nome do Corpo
    if(exibeNome)
    {
        glColor3f(0.5f, 0.5f, 0.9f);
        if(ehSol)
            desenhaTexto(0.0, 0.0, raioCorpo * 1.03, GLUT_BITMAP_HELVETICA_12, nomeCorpo);
        else
            desenhaTexto(0.0, 0.0, raioCorpo * escalaRaio * 1.03, GLUT_BITMAP_HELVETICA_12, nomeCorpo);
    }

	// Inclinação do Corpo nos eixos X e Y
    glRotatef(inclEixoX, 1.0f, 0.0f, 0.0f);
    glRotatef(inclEixoY, 0.0f, 1.0f, 0.0f);

    //  Desenha os eixos X, Y e Z
    if(exibeEixos)
    {
        if(ehSol)
            eixos3D(raioCorpo * 1.6f);
        else
            eixos3D(raioCorpo * escalaRaio * 1.6f);
    }

    // Emissão de luz
	if(ehSol)
	{
	    glColor3f(30.0f, 30.0f, 30.0f);
	    glDisable(GL_LIGHTING);
	}
	else
	{
		glColor3f(0.15f, 0.15f, 0.15f);
		glEnable(GL_LIGHTING);
	}

	glPushMatrix();

    // Aplica a textura
	glBindTexture (GL_TEXTURE_2D, idTextura);

    // Rotaciona o corpo em seu eixo
    glRotatef(rotacao, 0.0f, 0.0f, 1.0f);

	// Desenha a esfera
	if(ehSol) // Se for o Sol o raio não muda
        esfera(raioCorpo, 80, 40);
    else // Se não for o Sol escala o raio do corpo
        esfera(raioCorpo * escalaRaio, 80, 40);

	glPopMatrix();

	glPopMatrix();

}

// Desenha a órbita do corpo
void CorpoCeleste::desenhaOrbita(void)
{
    glDisable(GL_LIGHTING);
	float passo = 0.01f;
	double x1 = RX(0.0), x2, y1 = 0.0, y2;

    glColor3f(0.3f, 0.3f, 0.5f);

	for(float i = passo; i < M_2PI + passo; i = i + passo)
	{
		x2 = RX(i);
		y2 = RY(i);

		glBegin(GL_LINES);
            glVertex3f(P1 * x1 + P2 * y1, P3 * x1 + P4 * y1, P5 * x1 + P6 * y1);
            glVertex3f(P1 * x2 + P2 * y2, P3 * x2 + P4 * y2, P5 * x2 + P6 * y2);
		glEnd();

		x1 = x2;
		y1 = y2;
	}

	if(exibeInfoOrbita)
	{
		// Elementos da órbita - Afélio, Periélio, Nó Ascendente e Nó descendente
		float perielio_x = RX(0.0);
		float afelio_x = RX(M_PI);
		float NA_x = RX(-AP);
		float NA_y = RY(-AP);
		float ND_x = RX(M_PI - AP);
		float ND_y = RY(M_PI - AP);

		glBegin(GL_LINES);
		glVertex3f(P1 * perielio_x, P3 * perielio_x, P5 * perielio_x);
		glVertex3f(P1 * afelio_x, P3 * afelio_x, P5 * afelio_x);

		glVertex3f(P1 * NA_x + P2 * NA_y, P3 * NA_x + P4 * NA_y, P5 * NA_x + P6 * NA_y);
		glVertex3f(P1 * ND_x + P2 * ND_y, P3 * ND_x + P4 * ND_y, P5 * ND_x + P6 * ND_y);
		glEnd();

		glColor3f(1.0f, 0.0f, 0.0f);
        desenhaTexto(P1 * perielio_x, P3 * perielio_x, P5 * perielio_x, GLUT_BITMAP_HELVETICA_12, "P");
		desenhaTexto(P1 * afelio_x, P3 * afelio_x, P5 * afelio_x, GLUT_BITMAP_HELVETICA_12, "A");
		desenhaTexto(P1 * NA_x + P2 * NA_y, P3 * NA_x + P4 * NA_y, P5 * NA_x + P6 * NA_y, GLUT_BITMAP_HELVETICA_12, "NA");
		desenhaTexto(P1 * ND_x + P2 * ND_y, P3 * ND_x + P4 * ND_y, P5 * ND_x + P6 * ND_y, GLUT_BITMAP_HELVETICA_12, "ND");
	}

	glEnable(GL_LIGHTING);
}

// Desenha os eixos XYZ
void CorpoCeleste::eixos3D(float dim)
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3d(-dim, 0.0f, 0.0f);       glVertex3d(dim, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3d(0.0f, -dim, 0.0f);       glVertex3d(0.0f, dim, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3d(0.0f, 0.0f, -dim);       glVertex3d(0.0f, 0.0f, dim);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos3f(dim, 0.0f, 0.0f);  //  Desenha o Eixo X
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'X');
	glColor3f(0.0f, 1.0f, 0.0f);
	glRasterPos3f(0.0f, dim, 0.0f);  //  Desenha o Eixo Y
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'Y');
	glColor3f(0.0f, 0.0f, 1.0f);
	glRasterPos3f(0.0f, 0.0f, dim);  //  Desenha o Eixo Z
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'Z');
	glEnable(GL_LIGHTING);
}


// Atualiza o corpo pai e seus filhos
void CorpoCeleste::atualizaCorpo(void)
{
    // Movimento de rotação
    rotacao = fmod(360.0 * relogio / periodoRot, 360.0) + diffRot;

    // Se o corpo for Sol não calcula os elementos orbitais
    // O programa considera o Sol fixo no espaço, na posição (0, 0, 0)
    if(ehSol)
        return;

    // Calcula quantos séculos se passaram desde a época J2000
    double dT = (relogio - 946724336.0) / 3155760000.0;

    // Calcula os elementos orbitais
    SE = SE_0 + varSE * dT;
	NA = NA_0 + varNA * dT;
	IO = IO_0 + varIO * dT;
	LP = LP_0 + varLP * dT;
	EX = EX_0 + varEX * dT;
	LM = LM_0 + varLM * dT;

    // Calcula a Anomalia Média e o Argumento do Periélio
	AM = LM - LP;
    AP = LP - NA;

    // Calcula a anomalia excêntrica
	int MAX_CALC = 50;

	double AM = LM - LP;
	double dM, dE;

	AE = AM + EX * sin(AM);

	while( MAX_CALC-- > 0 )
	{
		dM = AM - (AE - EX * sin(AE));
		dE = dM / (1.0 - EX * cos(AE));
		if(fabs(dE) < 1.0E-10)
            break;
		AE += dE;
	}

	// Calcula os coeficientes para a posição XYZ do planeta
    P1 = cos(AP) * cos(NA) - sin(AP) * sin(NA) * cos(IO);
	P2 = -sin(AP) * cos(NA) - cos(AP) * sin(NA) * cos(IO);
	P3 = cos(AP) * sin(NA) + sin(AP) * cos(NA) * cos(IO);
	P4 = -sin(AP) * sin(NA) + cos(AP) * cos(NA) * cos(IO);
	P5 = sin(AP) * sin(IO);
	P6 = cos(AP) * sin(IO);

    // Calcula as posições R, X, Y e Z do planeta
	R = SE * (1.0 - EX * cos(AE));

	// Escala do Semi-eixo
    SE *= escalaOrbita;

	double rX = RX(AE);
	double rY = RY(AE);

   	posX = P1 * rX + P2 * rY;
	posY = P3 * rX + P4 * rY;
	posZ = P5 * rX + P6 * rY;

}

// Retorna a velocidade orbital em U.C./s
double CorpoCeleste::getVelOrbital()
{
    return sqrt(CONST_G2 * (corpoPai->massaCorpo + massaCorpo) * (2.0 / R - 1.0 / (SE / escalaOrbita)));
}

// Retorna a posição XYZ
void CorpoCeleste::getPosicao(double pos[3])
{
    pos[0] = posX;
    pos[1] = posY;
    pos[2] = posZ;
    
    if(corpoPai == NULL)
        return;

    double posPai[3] = {0.0};

    corpoPai->getPosicao(posPai);

    pos[0] += posPai[0];
    pos[1] += posPai[1];
    pos[2] += posPai[2];
}

// Construtor
CorpoCeleste::CorpoCeleste()
{
    SE = 0.0;
    LM = 0.0;
    EX = 0.0;
    LP = 0.0;
    IO = 0.0;
    NA = 0.0;
    P1 = 0.0;
	P2 = 0.0;
	P3 = 0.0;
	P4 = 0.0;
	P5 = 0.0;
	P6 = 0.0;
	posX = 0.0;
	posY = 0.0;
	posZ = 0.0;
	diffRot = 0.0;
    corpoPai = NULL;
    exibeEixos = false;
    exibeOrbita = true;
    exibeNome = true;
    escalaOrbita = 1.0;
    escalaRaio = 1.0;
    velTempo = 1.0;
}

// Destrutor
CorpoCeleste::~CorpoCeleste()
{

}


/*******************************************************************\
|                        Funções Auiliares                          |
\*******************************************************************/

// Carrega a imagem de textura em JPEG
void CorpoCeleste::carregarImagemJPEG(const char *filename, GLuint ID_textura)
{
/*
 * Copyright (c) 2005-2007 David HENRY
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

	FILE *fp = NULL;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW j;
	unsigned int i;
	GLubyte *imagem;

	/* Open image file */
	fp = fopen (filename, "rb");
	if (!fp)
    {
		fprintf (stderr, "\nERRO: arquivo '%s' nao encontrado!\n", filename);
		return;
    }

	/* Create and configure decompressor */
	jpeg_create_decompress (&cinfo);
	cinfo.err = jpeg_std_error (&jerr);
	jpeg_stdio_src (&cinfo, fp);

	/* Read header and prepare for decompression */
	jpeg_read_header (&cinfo, TRUE);
	jpeg_start_decompress (&cinfo);

	imagem = (GLubyte *)malloc (sizeof(GLubyte) * cinfo.image_width *
								cinfo.image_height * cinfo.num_components);
	if(imagem == NULL)
	{
		fprintf (stderr, "\nERRO: alocacao de memoria 'imagem'!\n");
		return;
    }

	/* Extract each scanline of the image */
	for (i = 0; i < cinfo.image_height; ++i)
    {
		j = (imagem +	((cinfo.image_height - (i + 1)) * cinfo.image_width * cinfo.num_components));
		jpeg_read_scanlines (&cinfo, &j, 1);
    }

	// Tratamento da textura para o OpenGL
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glBindTexture ( GL_TEXTURE_2D, ID_textura );

//	glTexImage2D(GL_TEXTURE_2D, 0, cinfo.num_components, cinfo.image_width,
//					cinfo.image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);

	gluBuild2DMipmaps (GL_TEXTURE_2D, cinfo.num_components, cinfo.image_width,
							cinfo.image_height, GL_RGB, GL_UNSIGNED_BYTE, imagem);


	/* Finish decompression and release memory */
	jpeg_finish_decompress (&cinfo);
	jpeg_destroy_decompress (&cinfo);

	fclose (fp);
}


// Função para desenhar uma esfera
void CorpoCeleste::esfera(GLfloat raio, int longitude, int latitude)
{
    GLUquadricObj* q = gluNewQuadric();
    gluQuadricDrawStyle(q, GLU_FILL);
    gluQuadricNormals(q, GLU_SMOOTH);
    gluQuadricTexture(q, GL_TRUE);
    gluSphere(q, raio, longitude, latitude);
    gluDeleteQuadric(q);
}

// Função para desenhar textos
void CorpoCeleste::desenhaTexto(float x, float y, float z, void *fonte, const char *string)
{
    glDisable(GL_LIGHTING);
	glRasterPos3f(x, y, z);
	while(*string) glutBitmapCharacter(fonte, *string++);
	glEnable(GL_LIGHTING);
}
