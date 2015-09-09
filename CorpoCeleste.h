/**
 * CorpoCeleste.h
 *
 * 12/02/2015
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
 *
 * Cria a classe "CorpoCeleste"
 */


#ifndef __CORPOCELESTE_H__
#define __CORPOCELESTE_H__

#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <jpeglib.h>

#define RAD_GRAU   	57.2957795130823208768	// Conversão de Radianos para grau
#define GRAU_RAD	0.01745329251994329577	// Conversão de grau para radiano
#define M_2PI 		6.28318530717958647693	// 2*Pi

/** Macros da posição XY do corpo na órbita */
#define RX(v) (SE * (cos(v) - EX))
#define RY(v) (SE * sqrt(1.0 - EX * EX) * sin(v))

#define CONST_UA    149597870.7 // Unidade Astronômica em km
#define R_TERRA     6371.0      // Radio da Terra em km
#define M_TERRA     5.97219E24  // Massa da Terra em Kg
#define CONST_G2 	1.541009233645079E-6    // G (constante de gravitação universal)
                                            // em U.M. / (U.C.³ * s²)
                                            //(6.67259E-11*M_TERRA/(R_TERRA³))


//#define UA_UC   23481.02901910372

/** Define o corpo celeste */
class CorpoCeleste
{
    public:
        // Default constructor
        CorpoCeleste();

        // Default destructor
        virtual ~CorpoCeleste();

        /** Ajustes */

        // Ajusta o relógio do sistema solar
        static void ajustaRelogio(time_t t) { relogio = (double)t; }

        // Carrega os dados do Corpo do arquivo
        void dadosCorpo(const char *nome_arq, unsigned int id_Corpo);

        // Adiciona a textura do corpo
        void textura(const char *nome_textura);

        // Adiciona o corpo Pai
        void setCorpoPai(CorpoCeleste *cPai) { corpoPai = cPai; }

        // Se verdadeiro exibe os eixos XYZ
        void setExibeEixos(bool h) { exibeEixos = h; }

         // Se verdadeiro exibe informações da órbita
        void setExibeInfoOrbita(bool h) { exibeInfoOrbita = h; }

        // Ajusta a rotação inicial
        void setDiffRot(double r) { diffRot = r; }

        // Se verdadeiro exibe o nome do corpo
        void setExibeNome(bool h) { exibeNome = h; }

        // Ajusta a escala do tamanho das órbitas
        static void setEscalaOrbita(float e) { escalaOrbita = e; }

        // Ajusta a escala do tamanho dos raios dos corpos celestes (exceto o Sol)
        static void setEscalaRaio(float e) { escalaRaio = e; }

        // Ajusta a velocidade do tempo
        static void setVelTempo(float vt) { velTempo = vt; }


        /** Ações */

        // Desenha o corpo
        void desenha();

        // Atualiza a posição do corpo
        void atualizaCorpo(void);

        // Atualiza o relógio do sistema
        static void atualizaRelogio(double step) { relogio += step * velTempo; }

        /** Retornos */

        // Retorna o nome do corpo
        const char *getNome() { return nomeCorpo; }

        // Retorna o raio do corpo
        float getRaio() { return raioCorpo; }

        // Retorna a massa do corpo
        float getMassa() { return massaCorpo; }

        // Retorna o ID do corpo
        int getID() { return idCorpo; }

        // Retorna o Corpo Pai
        CorpoCeleste *getPai() { return corpoPai; }

        // Retorna verdadeiro se estiver exibindo os eixos
        bool getExibeEixos() { return exibeEixos; }

        // Retorna verdadeiro se estiver exibindo informações da órbita
        bool getExibeInfoOrbita() { return exibeInfoOrbita; }

        // Retorna a escala do tamanho da órbita
        static float getEscalaOrbita() { return escalaOrbita; }

        // Retorna a escala do tamanho dos raios dos corpos celestes
        static float getEscalaRaio() { return escalaRaio; }

        // Retorna a veloidade do tempo
        static float getVelTempo() { return velTempo; }

        // Retorna posição XYZ
        void getPosicao(double pos[3]);

        // Retorna a velocidade orbital em U.C./s
        double getVelOrbital();

        // Retorna a distância entre o corpo e o seu pai
        double getDistanciaPai(void) { return R - corpoPai->raioCorpo - raioCorpo; }

        // Retorna o nome do pai
        const char *getNomePai() { return corpoPai->nomeCorpo; }

        // Retorna o relógio do sistema solar
        static time_t getRelogio() { return (time_t)relogio; }

    protected:
    private:

        char nomeCorpo[20]; // Nome
        float raioCorpo;    // Raio em U.C.
        double massaCorpo;  // Massa em U.M.
        float inclEixoX;    // Ângulo de inclinação dos eixos X em graus
        float inclEixoY;    // Ângulo de inclinação dos eixos Y em graus
        double periodoRot;  // Período de rotação em segundos
        double diffRot; // Ajusta o ângulo de rotação inicial em graus
        bool ehSol;     // Verdadeiro se o corpo for Sol, falso do contrário
        float rotacao;  // Angulo de rotação em graus

        double SE;  // Semi-Eixo Maior em U.C
        double LM;  // Longitude Média em radianos
        double EX;  // Excentricidade
        double LP;  // Longitude do Periélio em radianos
        double AP;  // Argumento do Periélio em radianos
        double IO;  // Inclinação Orbital em radianos
        double NA;  // Longitude do Nó ascendente em radianos
        double AE;  // Anomalia Excêntrica em radianos
        double AM;  // Anomalia Média em radianos

        double SE_0;  // Semi-Eixo Maior em U.C na época J200
        double LM_0;  // Longitude Média em radianos na época J200
        double EX_0;  // Excentricidade na época J200
        double LP_0;  // Longitude do Periélio em radianos na época J200
        double IO_0;  // Inclinação Orbital em radianos na época J200
        double NA_0;  // Longitude do Nó ascendente em radianos na época J200

        double varSE;  // Variação do Semi-Eixo Maior em U.C./século
        double varLM;  // Variação da Longitude Média em radianos/século
        double varEX;  // Variação da Excentricidade por século
        double varLP;  // Variação da Longitude do Periélio em radianos/século
        double varIO;  // Variação da Inclinação Orbital em radianos/século
        double varNA;  // Variação da Longitude do Nó ascendente em radianos/século

        double P1;	// Coeficiente da posição orbital do planeta
        double P2;	// Coeficiente da posição orbital do planeta
        double P3;	// Coeficiente da posição orbital do planeta
        double P4;	// Coeficiente da posição orbital do planeta
        double P5;	// Coeficiente da posição orbital do planeta
        double P6;	// Coeficiente da posição orbital do planeta
        double R;   // Distância do corpo ao pai
        double posX;    // Posição X do corpo
        double posY;    // Posição Y do corpo
        double posZ;    // Posição Z do corpo

        CorpoCeleste *corpoPai;  // Endereço do corpo pai

        bool exibeEixos;    // Se verdadeiro exibe os eixos XYZ
        bool exibeOrbita;   // Se verdadeiro exibe a órbita do corpo
        bool exibeInfoOrbita;  // Se verdadeiro exibe informações da órbita
        bool exibeNome;     // Se verdadeiro exibe o nome do corpo
        unsigned int idCorpo; // ID do corpo
        GLuint idTextura;	// ID para armazenar o nome da textura

        static double relogio;  // Relógio do sistema solar
        static float escalaOrbita;  // Escala do tamanho das órbitas
        static float escalaRaio;    // Escala do tamanho do raio dos corpos (exceto o Sol)
        static float velTempo;  // Velocidade do tempo

        /** Funções Auxiliáres **/

        // Desenha a órbita
        void desenhaOrbita();

        // Desenha os eixos XYZ
        void eixos3D(float d);

        // Carrega uma imagem em JPEG
        void carregarImagemJPEG(const char *filename, GLuint ID_textura);

        // Desenha uma esfera
        void esfera(GLfloat raio, int longitude, int latitude);

        // Desenha textos
        void desenhaTexto(float x, float y, float z, void *fonte, const char *string);
};

#endif // __CORPOCELESTE_H__
