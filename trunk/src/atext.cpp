 /*****************************************************************************
 * Astral3D -- 3D engine based on OpenGL and SDL.                             *
 * Copyright (C) 2005 Pavel Stupka <pavel.stupka@gmail.com>                   *
 *                                                                            *
 * This library is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU Lesser General Public                 *
 * License as published by the Free Software Foundation; either               *
 * version 2.1 of the License, or (at your option) any later version.         *
 *                                                                            *
 * This library is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
 * Lesser General Public License for more details.                            *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public           *
 * License along with this library; if not, write to the Free Software        *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA *
 *****************************************************************************/

#include "atext.h"

using namespace std;
namespace astral3d {

//-----------------------------------------------------------------------------
//  vytvoreni fontu
//-----------------------------------------------------------------------------

bool AText2D::buildFont(char *filename, int translate,
                                        int windowW, int windowH,
                                        int fontW, int fontH,
                                        int charW, int charH)
{
    this->windowWidth = windowW;
    this->windowHeight = windowH;
    this->translate = translate;
    this->width = (float)1.0 / (fontW/charW);
    this->height = (float)1.0 / (fontH/charH);
    this->charsInLine = (int) fontW / charW;
    this->sizeWidth = charW;
    this->sizeHeight = charH;

    if(!loadTexture(filename, &(this->texture)))
        return false;

    return true;
}

//-----------------------------------------------------------------------------
//  nacteni retezce popisujici font ze souboru
//-----------------------------------------------------------------------------

bool AText2D::loadString(char *filename)
{
    ifstream file;
    file.open(filename);

    if(!file.is_open())
    {
        stringstream foo;
        stringstream bar;
        foo << "AText2D::loadString(\""<<filename<<"\")";
        bar << "ifstream.open("<<filename<<")";
        setAstral3DError("Can't open file with the font description", foo.str(), bar.str());
        return false;
    }

    char c;
    stringstream foo;

    while(c != EOF)
    {
        c = file.get();
        if(c != '\n') foo << c;
    }

    setString(foo.str());

    file.close();

    return true;
}

//-----------------------------------------------------------------------------
//  obdoba funkce printf pro vypis textu
//-----------------------------------------------------------------------------

void AText2D::print(GLint x, GLint y, const char *str, ...)
{
    char buf[256];

    va_list list;

    if(str == NULL)
        return;

    va_start(list, str);
    vsprintf(buf, str, list);
    va_end(list);

    this->print(x, y, this->sizeWidth, this->sizeHeight, buf);
}

//-----------------------------------------------------------------------------
//  obdoba funkce printf pro vypis textu
//-----------------------------------------------------------------------------

void AText2D::print(GLint x, GLint y, int sizeW, int sizeH, const char *str, ...)
{
    char buf[256];

    va_list list;

    if(str == NULL)
        return;

    va_start(list, str);
    vsprintf(buf, str, list);
    va_end(list);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,this->windowWidth,0,this->windowHeight,-100,100);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x,y,0);

    for(unsigned int i = 0; i < strlen(buf); i++)
    {
        int znak = this->strFont.find(buf[i], 0);

        float cx = this->width*(znak % this->charsInLine);
        float cy = this->height*(znak / this->charsInLine);

        glBegin(GL_QUADS);
            glTexCoord2f(cx, 1.0-(cy+height));
            glVertex2f(0.0, 0.0);

            glTexCoord2f(cx+width, 1.0-(cy+height));
            glVertex2f(sizeW, 0.0);

            glTexCoord2f(cx+width, 1.0-cy);
            glVertex2f(sizeW, sizeH);

            glTexCoord2f(cx, 1.0-cy);
            glVertex2f(0.0, sizeH);
        glEnd();
        glTranslated(translate, 0, 0);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

} // namespace astral3d
