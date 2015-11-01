/*
 * Planeta.h
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
 * Sistema Solar v. 2.4 - 29/12/2013
 *
 * Cria a classe CPlaneta
 *
 * Dist�ncias em UC. (Unidade de Comprimento)
 * 						1 UC = Raio da Terra em metros;
 * Massas em UM. (Unidade de Massa)
 * 						1 UM = Massa da Terra em kg;
 * �ngulos em graus;
 * Tempo em segundos;
 * Taxas de varia��o em graus/s�culo;
 */

#ifndef _H_PLANETA_H_
#define _H_PLANETA_H_

#include <string.h>
#include <list>
#include "Auxiliar.h"

#define R_TERRA 	6371010.0 	// Raio da Terra em metros
#define R_TERRA_KM 	6371.01		// Raio da Terra em km
#define M_TERRA 	5.9736E24 	// Massa da terra em kg
#define UA_KM		149597870.7 // Unidade Astron�mica
#define G_UP 		1.5413657990853E-6	//(6.67259E-11*M_TERRA/(R_TERRA�))
										// G (constante de gravita��o universal)
										// em UM/UC�.

//// Enumera��o dos planetas
enum IDPLANETA {SOL, MERCURIO, VENUS, TERRA, TERRA_LUA, MARTE,
				JUPITER, SATURNO, URANO, NETUNO, PLUTAO, MAX_PLANETA};

//// Define a classe dos Planetas
class CPlaneta;

//// Cria a lista de planetas
typedef std::list<CPlaneta*> ListaPlaneta;

//// Macros da posi��o XY do planeta na �rbita
#define Rx(v)	(SE * escala_distancia * (cos(v) - EX))
#define Ry(v)	(SE * escala_distancia * sqrt(1.0 - EX * EX) * sin(v))

//// Define os eixos de inclina��o do planeta
#define EIXO_X 0	// Eixo X (1, 0, 0)
#define EIXO_Y 1	// Eixo Y (0, 1, 0)
#define EIXO_XY 2	// Eixo entre X e Y (1, 1, 0)

//// Define a classe CPlaneta
class CPlaneta
{
	private:

	char *nome;		// Nome do planeta
	float raio;		// Raio do planeta em UC
	float IE;		// Inclina��o do eixo de rota��o em graus
	double massa;	// Massa do planeta em UM
	double SE;		// Semi eixo da orbita em UC
	double NA;		// Longitude do n� ascendente em radiano
	double IO;		// Inclina��o da �rbita em radiano
	double LP;		// Longitude do peri�lio em radiano
	double EX;		// Excentricidade da �rbita
	double LM;		// Longitude m�dia em radiano
	double SE_0;	// Semi eixo na �poca J2000 da orbita em UC
	double NA_0;	// Longitude do n� ascendente na �poca J2000 em radiano
	double IO_0;	// Inclina��o da �rbita na �poca J2000 em radiano
	double LP_0;	// Longitude do peri�lio na �poca J2000 em radiano
	double EX_0;	// Excentricidade da �rbita na �poca J2000
	double LM_0;	// Longitude m�dia na �poca J2000
	double AM;		// Anomalia m�dia em radianos
	double AP;		// Argumento do peri�lio em radianos
	double var_SE;	// Taxa de vari��o do Semi eixo da orbita em UC/s�culo
	double var_NA;	// Taxa de varia��o do Longitude do n� ascendente em radiano/s�culo
	double var_IO;	// Taxa de varia��o da Inclina��o da �rbita em radiano/s�culo
	double var_LP;	// Taxa de varia��o da longitude do peri�lio em radiano/s�culo
	double var_EX;	// Taxa de varia��o da Excentricidade da �rbita por s�culo
	double var_LM;	// Taxa de varia��o da longitude m�dia em radianos/s�culo
	double periodo_rot;	// Per�odo de rota��o em segundos
	double rotacao;		// �ngulo da rota��o em graus
	double diff_rot;	// Corre��o do movimento de rota��o em graus
	double R;			// Dist�ncia do Planeta a seu pai
	double pos_x;		// Posi��o x do planeta em rela��o ao Pai
	double pos_y;		// Posi��o y do planeta em rela��o ao Pai
	double pos_z;		// Posi��o z do planeta em rela��o ao Pai
	double P1;			// Coeficiente da posi��o orbital do planeta
	double P2;			// Coeficiente da posi��o orbital do planeta
	double P3;			// Coeficiente da posi��o orbital do planeta
	double P4;			// Coeficiente da posi��o orbital do planeta
	double P5;			// Coeficiente da posi��o orbital do planeta
	double P6;			// Coeficiente da posi��o orbital do planeta
	double b;			// Par�metro adicional para �rbitas de J�pter a Plut�o
	double c;			// Par�metro adicional para �rbitas de J�pter a Plut�o
	double s;			// Par�metro adicional para �rbitas de J�pter a Plut�o
	double f;			// Par�metro adicional para �rbitas de J�pter a Plut�o
	bool exibe_nome;			// Verdadeiro se exibe o nome do planeta
	bool exibe_meridiano;		// Verdadeiro se exibe os meridianos e paralelos do planeta
	bool exibe_orbita;			// Verdadeiro se exibe a �rbita do planeta
	bool exibe_orbita_avancada;	// Verdadeiro se exibe os pontos de Peri�lio, Af�lio e N�s da �rbita
	bool exibe_eixo;			// Verdadeiro se exibe o eixo de rota��o do planeta
	bool exibe_plano_orbita;	// Verdadeiro se exibe o plano da �rbita do planeta
	bool exibe_plano_equador;	// Verdadeiro se exibe o plano do equador do planeta
	bool exibe_linha_pai;		// Verdadeiro se exibe a linha que liga o planeta pai ao filho
	bool orbita_equador_pai;	// Verdadeiro se a inclina��o da �rbita � em rela��o � ecl�ptica
	GLuint id_textura;			// ID para armazenar o nome da textura
	unsigned int eixo_IE;		// Eixo de inclina��o do planeta X, Y ou XY
	CPlaneta *PlanetaPai;		// Ponteiro para o planeta pai
	ListaPlaneta Satelites;		// Lista dos satelites
	unsigned int ID;			// ID do planeta

	static double relogio;			// Rel�gio do sistema solar
	static double vel_tempo;		// Escala do tempo
	static double escala_distancia;	// Escala de dist�ncia

	/* Desenha a �rbita do planeta */
	void DesenhaOrbita(void);


	public:

/// Ajustes

	/* Recebe os par�metros f�sicos do planeta */
	void ParametroPlaneta (const char *nome, float raio, double massa, float ie, unsigned int eixo_ie, IDPLANETA id);

	/* Recebe os par�metros orbitais do planeta */
	void ParametroOrbita (double se, double la, double lp, double EX, double io, double L);

	/* Recebe as varia��es dos par�metros orbitais do planeta em graus/s�culo */
	void VariacaoParametroOrbita (double var_se, double var_la, double var_lp, double var_e, double var_io, double var_L);

	/* Recebe os par�metros orbitais adicionais para J�piter at� Plut�o */
	void ParametroOrbitaAdicional(double B, double C, double S, double F);

	/* Recebe o per�odo de rota��o em segundos */
	void PeriodoRotacao(double r) { periodo_rot = r; }

	/* Ajusta a rota��o inicial do planeta em graus */
	void AjustaRotacao(double rot) { diff_rot = rot; }

	/* Mostra o meridiano se 'h' for true e esconde se for falso */
	void ExibirMeridiano(bool h) { exibe_meridiano = h; }

	/* Mostra a �rbita se 'h' for true e esconde se for falso */
	void ExibirOrbita(bool h) { exibe_orbita = h; }

	/* Exibe o peri�lio, af�lio, n� ascendente e descendente se 'h' for true e esconde se for falso */
	void ExibirOrbitaAvancada(bool h) { exibe_orbita_avancada = h; }

	/* Exibe os eixos se 'h' for true e esconde se for falso */
	void ExibirEixo(bool h) { exibe_eixo = h; }

	/* Exibe o plano da �rbita se 'h' for true e esconde se for falso */
	void ExibirPlanoOrbita(bool h)	{ exibe_plano_orbita = h; }

	/* Exibe o plano do equador se 'h' for true e esconde se for falso */
	void ExibirPlanoEquador(bool h) { exibe_plano_equador = h; }

	/* Exibe uma linha do planeta at� o seu planeta pai se 'h' for true e esconde se for falso */
	void ExibirLinhaPai(bool h) { exibe_linha_pai = h; }

	/* Exibe o nome do planeta se 'h' for true e esconde se for falso */
	void ExibirNome(bool h) { exibe_nome = h; }

	/* Ajusta a hora do sistema solar */
	void AjustaRelogio(time_t t) { relogio = (double)t; }

	/* Ajusta a velocidade do tempo */
	void VelocidadeTempo(double t) { vel_tempo = t; }

	/* Ajusta a escala de dist�ncia entre os planetas */
	void EscalaDistancia(double d) { escala_distancia = d; }

	/* Se 'h' for true a �rbita do sat�lite � em rela��o ao equador do planeta pai
	 * Se for falso a �rbita � em rela��o ao plano da �rbita do planeta pai */
	void OrbitaEquadorPai(bool h) { orbita_equador_pai = h; }

	/* Atualiza o rel�gio do sistema solar */
	void AtualizaRelogio(double passo_tempo) { relogio += passo_tempo * vel_tempo; }

	/* Recebe o caminho para o arquivo da textura do planeta */
	void Textura(const char *nome_textura);

/// Retornos

	/* Retorna true se os eixos est�o exibidos e false se n�o */
	bool ExibirEixo(void) { return exibe_eixo; }

	/* Retorna true se meridianos est�o exibidos e false se n�o */
	bool ExibirMeridiano(void) { return exibe_meridiano; }

	/* Retorna true se o plano da �rbita est� exibida e false se n�o */
	bool ExibirPlanoOrbita(void) { return exibe_plano_orbita; }

	/* Retorna true se o plano do equador est� exibido e false se n�o */
	bool ExibirPlanoEquador(void) { return exibe_plano_equador; }

	/* Retorna true se a �rbita est� exibida e false se n�o */
	bool ExibirOrbita(void) { return exibe_orbita; }

	/* Retorna true se as informa��es da �rbita est�o exibidas e false se n�o */
	bool ExibirOrbitaAvancada(void) { return exibe_orbita_avancada; }

	/* Retorna true se a linha at� o planeta pai est� exibida e false se n�o */
	bool ExibirLinhaPai(void) { return exibe_linha_pai; }

	/* Retorna true se o nome do planeta est� exibido e false se n�o */
	bool ExibirNome(void) { return exibe_nome; }

	/* Retorna a velocidade do tempo do sistema solar */
	double VelocidadeTempo(void) { return vel_tempo; }

	/* Retorna a escala da dist�ncia entre os planetas */
	double EscalaDistancia(void) { return escala_distancia; }

	/* Retorna o relogio do sistema solar */
	time_t Relogio(void) { return (time_t)relogio; }

	/* Retorna a velocidade orbital do planeta em U.P./s */
	double VelocidadeOrbital(void) {
		return sqrt(G_UP * (PlanetaPai->massa + massa) * (2.0 / R - 1.0 / SE));
	}

	/* Retorna o semi eixo maior do planeta */
	double SemiEixo(void) { return SE; }

	/* Retorna a longitude do n� ascendente do planeta em graus */
	double NoAscendente(void) { return fmod(NA * RAD_GRAU, 360.0); }

	/* Retorna o argumento do peri�lio do planeta em graus */
	double ArgumentoPerielio(void) { return fmod(AP * RAD_GRAU, 360.0); }

	/* Retorna a excentricidade da �rbita do planeta */
	double Excentricidade(void) { return EX; }

	/* Retorna a inclina��o da �rbita do planeta em graus */
	double InclinacaoOrbital(void) { return fmod(IO * RAD_GRAU, 360.0); }

	/* Retorna a anomalia m�dia do planeta em graus */
	double AnomaliaMedia(void) { return fmod(AM * RAD_GRAU, 360.0); }

	/* Retorna o raio do planeta */
	float Raio(void) { return raio; }

	/* Retorna a massa do planeta */
	double Massa(void) { return massa; }

	/* Retorna a dist�ncia entre o planeta e o seu pai */
	double DistanciaPai(void) { return R - PlanetaPai->raio - raio; }

	/* Retorna o nome do planeta */
	const char*	Nome(void) { return nome; }

	/* Retorna o nome do planeta pai */
	const char* NomePai(void) { return PlanetaPai->nome; }

	/* Retorna o per�odo de rota��o do planeta em segundos */
	double PeriodoRotacao(void) { return periodo_rot; }

	/* Per�odo de transla��o do planeta em segundos */
	double PeriodoTranslacao(void) {
		return M_2PI * SE * sqrt(SE / (G_UP * (PlanetaPai->massa + massa)));
	}

	/* Retorna a posi��o global XYZ do planeta */
	void Posicao(double *x, double *y, double *z);

///
	/* Cria um novo planeta para o seu pai e retorna um ponteiro */
	CPlaneta *AdicionaSatelite(void);

	/* Desenha os planetas */
	void DesenhaPlanetas(void);

	/* Calcula as posi��es dos planetas */
	void MovimentoPlanetas(void);

	CPlaneta();
	~CPlaneta();
};

double CPlaneta::relogio; /* Rel�gio do sistama solar */
double CPlaneta::vel_tempo; /* Velocidade do tempo */
double CPlaneta::escala_distancia; /* Escala da dist�ncia entre os planetas */

//////////////////////////////////////////////////////////////////////////
//                        Ajustes iniciais								//
//////////////////////////////////////////////////////////////////////////

CPlaneta::CPlaneta()
{
	massa = 0.0;
	raio = 0.0f;
	SE = 0.0;
	NA = 0.0;
	IO = 0.0;
	IE = 0.0;
	LP = 0.0;
	EX = 0.0;
	SE_0 = 0.0;
	NA_0 = 0.0;
	LP_0 = 0.0;
	EX_0 = 0.0;
	IO_0 = 0.0;
	periodo_rot = 0.0;
	AM = 0.0;
	AP = 0.0;
	LM = 0.0;
	LM_0 = 0.0;
	var_SE = 0.0;
	var_NA = 0.0;
	var_IO = 0.0;
	var_LP = 0.0;
	var_EX = 0.0;
	var_LM = 0.0;
	rotacao = 0.0;
	diff_rot = 0.0;
	nome = NULL;
	exibe_meridiano = false;
	exibe_nome = false;
	exibe_orbita = false;
	exibe_orbita_avancada = false;
	exibe_eixo = false;
	exibe_plano_orbita = false;
	exibe_plano_equador = false;
	exibe_linha_pai = false;
	vel_tempo = 1.0;
	escala_distancia = 1.0;
	PlanetaPai = NULL;
	R = 0.0;
	pos_x = 0.0;
	pos_y = 0.0;
	pos_z = 0.0;
	orbita_equador_pai = false;
	P1 = 0.0;
	P2 = 0.0;
	P3 = 0.0;
	P4 = 0.0;
	P5 = 0.0;
	P6 = 0.0;
	b = 0.0;
	c = 0.0;
	s = 0.0;
	f = 0.0;
	id_textura = 0;
	eixo_IE = EIXO_X;
}

//////////////////////////////////////////////////////////////////////////
//            Fun��o para desenha a �rbita do planeta 					//
//////////////////////////////////////////////////////////////////////////

void CPlaneta::DesenhaOrbita(void)
{
	float passo = 0.01f;

	glColor3f(0.0f, 0.0f, 1.0f);

	double x1, x2, y1, y2;

	x1 = Rx(0.0);
	y1 = 0.0;

	for( float i = passo; i < M_2PI + passo; i = i + passo )
	{
		x2 = Rx(i);
		y2 = Ry(i);

		glBegin(GL_LINES);
		glVertex3f(P1 * x1 + P2 * y1, P3 * x1 + P4 * y1, P5 * x1 + P6 * y1);
		glVertex3f(P1 * x2 + P2 * y2, P3 * x2 + P4 * y2, P5 * x2 + P6 * y2);
		glEnd();

		x1 = x2;
		y1 = y2;
	}

	if(exibe_orbita_avancada)
	{
		// Elementos da �rbita - Af�lio, Peri�lio, N� Ascendente e N� descendente
		float perielio_x = Rx(0.0);
		float afelio_x = Rx(M_PI);
		float NA_x = Rx(-AP);
		float NA_y = Ry(-AP);
		float ND_x = Rx(M_PI - AP);
		float ND_y = Ry(M_PI - AP);

		glBegin(GL_LINES);
		glVertex3f(P1 * perielio_x, P3 * perielio_x, P5 * perielio_x);
		glVertex3f(P1 * afelio_x, P3 * afelio_x, P5 * afelio_x);

		glVertex3f(P1 * NA_x + P2 * NA_y, P3 * NA_x + P4 * NA_y, P5 * NA_x + P6 * NA_y);
		glVertex3f(P1 * ND_x + P2 * ND_y, P3 * ND_x + P4 * ND_y, P5 * ND_x + P6 * ND_y);
		glEnd();

		glColor3f(1.0f, 0.0f, 0.0f);
		DesenhaTexto(P1 * perielio_x, P3 * perielio_x, P5 * perielio_x, GLUT_BITMAP_HELVETICA_12, "P");
		DesenhaTexto(P1 * afelio_x, P3 * afelio_x, P5 * afelio_x, GLUT_BITMAP_HELVETICA_12, "A");
		DesenhaTexto(P1 * NA_x + P2 * NA_y, P3 * NA_x + P4 * NA_y, P5 * NA_x + P6 * NA_y, GLUT_BITMAP_HELVETICA_12, "NA");
		DesenhaTexto(P1 * ND_x + P2 * ND_y, P3 * ND_x + P4 * ND_y, P5 * ND_x + P6 * ND_y, GLUT_BITMAP_HELVETICA_12, "ND");
	}
}

//////////////////////////////////////////////////////////////////////////
//     Atualiza os movimentos de rota��o e transla��o do Planeta		//
//////////////////////////////////////////////////////////////////////////

void CPlaneta::MovimentoPlanetas(void)
{
	// Movimento de rota��o
	rotacao = fmodf(relogio / periodo_rot * 360.0f, 360.0f ) + diff_rot;

	// Movimento de transla��o se n�o for o Sol
	if(ID != SOL)
	{
		// Atualiza Elementos keplerianos
		double T = (relogio - 946735136.0) / 3155760000.0;

		SE = SE_0 + var_SE * T;
		NA = NA_0 + var_NA * T;
		IO = IO_0 + var_IO * T;
		LP = LP_0 + var_LP * T;
		EX = EX_0 + var_EX * T;
		LM = LM_0 + var_LM * T;

		AM = LM - LP + b * T* T + c * cos(f * T) + s * sin(f * T);
		AP = LP - NA;

		// Calcula a anomalia exc�ntrica
		int MAX_CALC = 50;

		double dM, dE, E = AM + EX * sin(AM);

		while( --MAX_CALC > 0 )
		{
			dM = AM - (E - EX * sin(E));
			dE = dM / (1.0 - EX * cos(E));
			if(fabs(dE) < 1.0E-10) break;
			E = E + dE;
		}

		// Calcula os coeficientes para a posi��o XYZ do planeta
		P1 = cos(AP) * cos(NA) - sin(AP) * sin(NA) * cos(IO);
		P2 = -sin(AP) * cos(NA) - cos(AP) * sin(NA) * cos(IO);
		P3 = cos(AP) * sin(NA) + sin(AP) * cos(NA) * cos(IO);
		P4 = -sin(AP) * sin(NA) + cos(AP) * cos(NA) * cos(IO);
		P5 = sin(AP) * sin(IO);
		P6 = cos(AP) * sin(IO);

		// Calcula as posi��es R, X, Y e Z do planeta
		R = SE * (1.0 - EX * cos(E));
		double R_x = Rx(E);
		double R_y = Ry(E);

		pos_x = P1 * R_x + P2 * R_y;
		pos_y = P3 * R_x + P4 * R_y;
		pos_z = P5 * R_x + P6 * R_y;
	}

	// Atualiza os movimentos de rota��o e transla��o dos Sat�lites
	for (ListaPlaneta::iterator sat = Satelites.begin(); sat != Satelites.end(); sat++)
	{
		CPlaneta* satelite = *sat;
		satelite->MovimentoPlanetas();
	}
}

//////////////////////////////////////////////////////////////////////////
//           Recebe os par�metros f�sicos do planeta 					//
//////////////////////////////////////////////////////////////////////////

void CPlaneta::ParametroPlaneta( const char *n, float r, double m, float ie, unsigned int eixo_ie, IDPLANETA id )
{
	nome = new char[strlen(n) + 1];

	if(nome == NULL)
	{
		printf("\n\nERRO na alocacao de memoria para o nome do planeta.\n\n");
		exit(1);
	}
	strcpy(nome, n);	// Nome do planeta
	raio = r;			// Raio do planeta em UC
	massa = m;			// Massa do planeta em UM
	IE = ie;			// Inclina��o do eixo de rota��o em graus
	eixo_IE = eixo_ie;	// Eixo de inclina��o do planeta (X ou Y)
	ID = id;			// ID do planeta
}

//////////////////////////////////////////////////////////////////////////
//            Recebe os par�metros da �rbita na �poca J2000				//
//////////////////////////////////////////////////////////////////////////

void CPlaneta::ParametroOrbita( double se, double la, double lp, double ex, double io, double L )
{
	SE_0 = se;				// Semi-eixo maior em UC na �poca J2000
	NA_0 = la * GRAU_RAD;	// Longitude do n� ascendente em graus na �poca J2000
	LP_0 = lp * GRAU_RAD;	// Longitude do peri�lio em graus na �poca J2000
	EX_0 = ex;				// Excentricidade da �rbita na �poca J2000
	IO_0 = io * GRAU_RAD;	// Inclina��o da �rbita em graus na �poca J2000
	LM_0 = L * GRAU_RAD;	// Longitude m�dia em graus na �poca J2000
}

//////////////////////////////////////////////////////////////////////////
//         Ajusta as taxas de varia��o dos parametros orbitais 			//
//////////////////////////////////////////////////////////////////////////

void CPlaneta::VariacaoParametroOrbita (double var_se, double var_la, double var_lp, double var_exc, double var_io, double var_L)
{
	var_SE = var_se;			// Taxa de varia��o do semi-eixo maior em UC/s�culo
	var_NA = var_la * GRAU_RAD;	// Taxa de varia��o da longitude do n� ascendente em graus/s�culo
	var_IO = var_io * GRAU_RAD;	// Taxa de varia��o da inclina��o da �rbita em graus/s�culo
	var_LP = var_lp * GRAU_RAD;	// Taxa de varia��o da longitude do peri�lio em graus/s�culo
	var_EX = var_exc;			// Taxa de varia��o da excentricidade em graus/s�culo
	var_LM = var_L * GRAU_RAD;	// Taxa de varia��o da Longitude m�dia em graus/s�culo
}

//////////////////////////////////////////////////////////////////////////////
// Recebe os par�metros orbitais adicionais dos planetas J�piter at� Plut�o //
//////////////////////////////////////////////////////////////////////////////

void CPlaneta::ParametroOrbitaAdicional(double B, double C, double S, double F)
{
	b = B * GRAU_RAD;
	c = C * GRAU_RAD;
	s = S * GRAU_RAD;
	f = F * GRAU_RAD;
}

//////////////////////////////////////////////////////////////////////////
//                       Desenha os planetas 							//
//////////////////////////////////////////////////////////////////////////

void CPlaneta::DesenhaPlanetas(void)
{
	glPushMatrix();

	// Transla��o e �rbita se n�o for o Sol
	if(ID != SOL)
	{
		// Ajusta a �rbita do planeta filho para o equador do planeta pai ou ecliptica
		if(orbita_equador_pai)
		{
			if(PlanetaPai->eixo_IE == EIXO_X) glRotatef(PlanetaPai->IE, 1.0f, 0.0f, 0.0f);
			else if(eixo_IE == EIXO_Y) glRotatef(PlanetaPai->IE, 0.0f, 1.0f, 0.0f);
				 else if(eixo_IE == EIXO_XY) glRotatef(IE, 1.0f, 1.0f, 0.0f);
		}

		// Desenha a �rbita
		if(exibe_orbita) DesenhaOrbita();

		// Desenha a linha at� o planeta pai
		if(exibe_linha_pai)
		{
			glColor3f(1.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
				glVertex3f(0.0f, 0.0f, 0.0f);	glVertex3f(pos_x, pos_y, pos_z);
			glEnd();
		}

		// Translada o planeta para a posi��o na �rbita
		glTranslatef(pos_x, pos_y, pos_z);
	}

	// Exibe o nome do planeta
	if(exibe_nome)
	{
		glColor3f(10.0f, 10.0f, 10.0f);
		DesenhaTexto(0, 0, 0, GLUT_BITMAP_HELVETICA_12, nome);
	}

	// Desenha o plano da �rbita
	if(exibe_plano_orbita) malha(5.0f * raio, 10);

	glPushMatrix();

	// Inclina��o do eixo de rota��o do planeta
	if(eixo_IE == EIXO_X) glRotatef(IE, 1.0f, 0.0f, 0.0f);
	else if(eixo_IE == EIXO_Y) glRotatef(IE, 0.0f, 1.0f, 0.0f);
		 else if(eixo_IE == EIXO_XY) glRotatef(IE, 1.0f, 1.0f, 0.0f);

	// Desenha o plano do equador
	if( exibe_plano_equador ) malha(5.0f * raio, 10);

	// Exibe os eixos
	if( exibe_eixo ) eixo(raio * 1.6f);

	// Rota��o em torno do pr�prio eixo
	glRotatef(rotacao, 0.0f, 0.0f, 1.0f);

	// Exibe os meridianos e paralelos
	if(exibe_meridiano)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glutWireSphere(raio*1.02, 18, 12);
	}

	// Aumenta o brilho e n�o liga a luz se for o Sol
	if(ID == SOL) glColor3f(30.0f, 30.0f, 30.0f);
	else
	{
		glColor3f(0.15f, 0.15f, 0.15f);
		glEnable(GL_LIGHTING);
	}

	// Liga a Textura
	glEnable(GL_TEXTURE_2D);

	// Aplica a textura do planeta
	glBindTexture (GL_TEXTURE_2D, id_textura);

	// Desenha o planeta
	Esfera(raio, 80, 40);

	// Desliga ilumina��o e textura
	if(glIsEnabled(GL_LIGHTING)) glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	// Desenha os satelites
	for (ListaPlaneta::iterator sat = Satelites.begin(); sat != Satelites.end(); sat++)
	{
		CPlaneta* satelite = *sat;
		satelite->DesenhaPlanetas();
	}

	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////
//       Cria um novo planeta para o pai e retorna um ponteiro			//
//////////////////////////////////////////////////////////////////////////

CPlaneta *CPlaneta::AdicionaSatelite(void)
{
	CPlaneta* satelite = new CPlaneta;

	if(satelite == NULL)
	{
		printf("\n\nErro AdicionaSatelite: Falha na alocacao de memoria.\n\n");
		exit(1);
	}

	satelite->PlanetaPai = this;
	Satelites.push_back(satelite);

	return satelite;
}

//////////////////////////////////////////////////////////////////////////
//               Retorna a posi��o global XYZ do planeta 				//
//////////////////////////////////////////////////////////////////////////

void CPlaneta::Posicao(double *x, double *y, double *z)
{
	if(PlanetaPai == NULL)
	{
		*x = 0.0;
		*y = 0.0;
		*z = 0.0;
		return;
	}

	double px, py, pz;

	PlanetaPai->Posicao(&px, &py, &pz);

	*x = px + pos_x;
	*y = py + pos_y;
	*z = pz + pos_z;
}

//////////////////////////////////////////////////////////////////////////
//                Carrega a textura do planeta 							//
//////////////////////////////////////////////////////////////////////////

void CPlaneta::Textura(const char *nome_textura)
{
	glGenTextures(1, &id_textura);
	CarregarImagemJPEG(nome_textura, id_textura);
}

//////////////////////////////////////////////////////////////////////////
//                      Destroi o planeta								//
//////////////////////////////////////////////////////////////////////////

CPlaneta::~CPlaneta(void)
{
	delete nome;
	glDeleteTextures(1, &id_textura);
	Satelites.clear();
}

#endif	// _H_PLANETA_H_

