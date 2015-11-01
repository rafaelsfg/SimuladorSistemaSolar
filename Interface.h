/*
 * Interface.h
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
 * 12/05/2012
 *
 * Contém a função para desenhar a interface com informações ao usuário
 */

#ifndef _H_INTERFACE_H_
#define _H_INTERFACE_H_

extern CPlaneta *Planeta[MAX_PLANETA]; // Vetor dos planetas
extern float distancia; // Distancia do observador ao planeta/objeto
extern IDPLANETA poscamera; // Posição da câmera no Planeta/objeto


// Se true exibe informações extras das órbitas dos planetas
bool exibe_info_extra = false;

//////////////////////////////////////////////////////////////////////////
//          Função para desenhar as informações na tela 				//
//////////////////////////////////////////////////////////////////////////

void InformacaoNaTela(void)
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

	// Desenha o relogio
	time_t relogio = Planeta[SOL]->Relogio();
	struct tm * timeinfo = localtime ( &relogio );
	strftime (buffer, 80, "%d/%m/%Y - %H:%M:%S", timeinfo);
	glColor3f(1.0f, 1.0f, 1.0f);
	DesenhaTexto(glutGet(GLUT_WINDOW_WIDTH) - 200, 30, 0, GLUT_BITMAP_HELVETICA_18, buffer);

	// Desenha a velocidade do tempo
	float vel = Planeta[SOL]->VelocidadeTempo();
	if(vel == 0.0)
	{
		sprintf(buffer, "Tempo parado");
	}
	else
	{
		if(fabs(vel) > 1.0)
		{
			sprintf(buffer, "%gx mais depressa", vel);
		}
		else
		{
			if(fabs(vel) < 1.0)
			{
				sprintf(buffer, "%gx mais devagar", 1.0f/vel);
			}
			else
			{
				if(vel == 1.0)
				{
					sprintf(buffer, "Tempo real");
				}
				else
				{
					sprintf(buffer, "Tempo inverso");
				}
			}
		}
	}
	DesenhaTexto(glutGet(GLUT_WINDOW_WIDTH) - 200, 50, 0, GLUT_BITMAP_HELVETICA_12, buffer);

	//Informações do planeta

	//Nome
	glColor3f(1.0f, 0.0f, 0.0f);
	DesenhaTexto(20.0, 30.0, 0.0, GLUT_BITMAP_TIMES_ROMAN_24, Planeta[poscamera]->Nome());

	//Raio
	glColor3f(1.0f, 0.5f, 0.0f);
	sprintf(buffer, "Raio medio: %G km", Planeta[poscamera]->Raio() * R_TERRA_KM);
	DesenhaTexto(20.0, 60.0, 0.0, GLUT_BITMAP_HELVETICA_12, buffer);

	// Massa
	sprintf(buffer, "Massa: %G kg", Planeta[poscamera]->Massa() * M_TERRA);
	DesenhaTexto(20.0, 75.0, 0.0, GLUT_BITMAP_HELVETICA_12, buffer);

	// Distância
	sprintf(buffer, "Distancia: %G km", (distancia - Planeta[poscamera]->Raio()) * R_TERRA_KM);
	DesenhaTexto(20.0, 90.0, 0.0, GLUT_BITMAP_HELVETICA_12, buffer);

	if(poscamera != SOL)
	{
		//Velocidade orbital
		sprintf(buffer, "Velocidade orbital: %g km/s", Planeta[poscamera]->VelocidadeOrbital() * R_TERRA_KM);
		DesenhaTexto(20, 105, 0, GLUT_BITMAP_HELVETICA_12, buffer);

		// Distância do pai
		float d = Planeta[poscamera]->DistanciaPai() * R_TERRA_KM;
		sprintf(buffer, "Distancia (%s): %.1f km (%.4f UA)", Planeta[poscamera]->NomePai(), d, d / UA_KM);
		DesenhaTexto(20, 120, 0, GLUT_BITMAP_HELVETICA_12, buffer);
	}

	// Exibe informações extras
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

	// Escala da distância
	sprintf(buffer, "Escala de distancia: 1:%G", 1.0 / Planeta[SOL]->EscalaDistancia());
	DesenhaTexto(20, glutGet(GLUT_WINDOW_HEIGHT) - 20, 0, GLUT_BITMAP_HELVETICA_12, buffer);


	// Volta a tela para 3D
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

#endif	// _H_INTERFACE_H_
