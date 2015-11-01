/*
 * Auxiliar.h
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
 * 29/12/2013
 * Contém funções auxiliares ao programa
 */


#ifndef _H_AUXILIAR_H_
#define _H_AUXILIAR_H_

#include <math.h>
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

#define RAD_GRAU    57.2957795130823208768  // Conversão de Radianos para grau
#define GRAU_RAD    0.01745329251994329577  // Conversão de grau para radiano
#define M_2PI       6.28318530717958647693  // 2*Pi

//////////////////////////////////////////////////////////////////////////
//             Carrega a imagem de textura em JPEG                      //
//////////////////////////////////////////////////////////////////////////

void CarregarImagemJPEG(const char *filename, GLuint ID_textura)
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
        j = (imagem +   ((cinfo.image_height - (i + 1)) * cinfo.image_width * cinfo.num_components));
        jpeg_read_scanlines (&cinfo, &j, 1);
    }

    // Tratamento da textura para o OpenGL
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glBindTexture ( GL_TEXTURE_2D, ID_textura );

//  glTexImage2D(GL_TEXTURE_2D, 0, cinfo.num_components, cinfo.image_width,
//                  cinfo.image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);

    gluBuild2DMipmaps (GL_TEXTURE_2D, cinfo.num_components, cinfo.image_width,
                            cinfo.image_height, GL_RGB, GL_UNSIGNED_BYTE, imagem);


    /* Finish decompression and release memory */
    jpeg_finish_decompress (&cinfo);
    jpeg_destroy_decompress (&cinfo);

    fclose (fp);
}

/////////////////////////////////////////////////////////////////////////////
// Função para desehar uma malha de tamanho 'dimensao' com 'ndiv' divisões //
/////////////////////////////////////////////////////////////////////////////

void malha(float dimensao, int ndiv)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-dimensao, -dimensao, 0.0f);
        glVertex3f(-dimensao, dimensao, 0.0f);
        glVertex3f(dimensao, dimensao, 0.0f);
        glVertex3f(dimensao, -dimensao, 0.0f);
    glEnd();

    for(int i = 1; i <= ndiv; i++)
    {
        glBegin(GL_LINES);
            glVertex3f(-dimensao, dimensao - (float)i * 2.0f * dimensao / ndiv, 0.0f);
            glVertex3f(dimensao, dimensao - (float)i * 2.0f * dimensao / ndiv, 0.0f);
            glVertex3f(dimensao - (float)i * 2.0f * dimensao / ndiv, dimensao, 0.0f);
            glVertex3f(dimensao - (float)i * 2.0f * dimensao / ndiv, -dimensao, 0.0f);
        glEnd();
    }
}

//////////////////////////////////////////////////////////////////////////
//             Função para desenhar os Eixos X, Y e Z                   //
//////////////////////////////////////////////////////////////////////////

void eixo(float r)
{
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-r, 0.0f, 0.0f);       glVertex3f(r * 1.5f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -r, 0.0f);       glVertex3f(0.0f, r * 1.5f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -r);       glVertex3f(0.0f, 0.0f, r * 1.5f);
    glEnd();
}

//////////////////////////////////////////////////////////////////////////
//                 Função para desenhar uma esfera                      //
//////////////////////////////////////////////////////////////////////////

void Esfera(GLfloat raio, int longitude, int latitude)
{
  GLUquadricObj* q = gluNewQuadric ( );
  gluQuadricDrawStyle ( q, GLU_FILL );
  gluQuadricNormals ( q, GLU_SMOOTH );
  gluQuadricTexture ( q, GL_TRUE );
  gluSphere ( q, raio, longitude, latitude );
  gluDeleteQuadric ( q );
}

/////////////////////////////////////////////////////////////////////////////////
// Função para desenhar um anel que inicia de 'r0' até 'r1' com 'div' divisões //
/////////////////////////////////////////////////////////////////////////////////

void Anel(float r0, float r1, int div)
{
    if(div < 4.0) return;

    float passo = M_2PI / (float)div;

    for(float i = 0.0; i < M_2PI; i += passo)
    {
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(r0 * cos(i), r0 * sin(i), 0.0);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(r1 * cos(i), r1 * sin(i), 0.0);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(r1 * cos(i + passo), r1 * sin(i + passo), 0.0);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(r0 * cos(i + passo), r0 * sin(i + passo), 0.0);
        glEnd();
    }
}

//////////////////////////////////////////////////////////////////////////
//           Cria um retângulo de lado 'l' e altura 'h'                 //
//////////////////////////////////////////////////////////////////////////

void Retangulo(float l, float h)
{
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( l / 2.0, -h / 2.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( l / 2.0, h / 2.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -l / 2.0f,  h / 2.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( -l / 2.0f, -h / 2.0f, 0.0f);
    glEnd();
}

//////////////////////////////////////////////////////////////////////////
//        Função para desenhar texto na tela na posição XYZ             //
//////////////////////////////////////////////////////////////////////////

void DesenhaTexto(float x, float y, float z, void *fonte, const char *string)
{
    glRasterPos3f(x, y, z);
    while(*string) glutBitmapCharacter(fonte, *string++);
}

//////////////////////////////////////////////////////////////////////////
//                 Retorna a norma do vetor XYZ                         //
//////////////////////////////////////////////////////////////////////////

double norma( double x, double y, double z)
{
    return sqrt(x * x + y * y + z * z);
}

#endif  // _H_AUXILIAR_H_
