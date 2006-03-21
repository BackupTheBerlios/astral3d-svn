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

#include "aconsole.h"

using namespace std;
namespace astral3d {

//-----------------------------------------------------------------------------
// konstruktor
//-----------------------------------------------------------------------------

AConsole::AConsole()
{
    text2D = NULL;
    lineBuffer = NULL;
    visible = true;
    consoleOnOffKey = SDLK_F1;
    r=g=b=255;
    fadeOut=false;
    userInput = true;
    isTextured = false;
    func = NULL;
    backgroundAlpha = 255;
}

//-----------------------------------------------------------------------------
// rusi konzoli
//-----------------------------------------------------------------------------

void AConsole::destroy()
{
    text2D = NULL;
    if(lineBuffer)
        delete[] lineBuffer;
    lineBuffer = NULL;
    deleteTexture(&texture);
}

//-----------------------------------------------------------------------------
// vytvari konzoli
//-----------------------------------------------------------------------------

bool AConsole::build(AText2D *text2D, int x, int y, int w, int h, int border)
{
    if(!text2D)
        return false;

    // nastaveni promennych konzole
    this->text2D = text2D;
    width = w;
    height = h;
    posX = x;
    posY = y;
    this->border = border;

    // kolik se vejde znaku na radek a radku do konzole
    maxLineChars = (int) ((width-border*2) / text2D->getTranslate());
    noOfLines = (int) ((height-border*2) / text2D->getSizeHeight());

    // jedna radka pro prikazovou radku
    // noOfLines--;
    // ale pokud uzivatel vypne vstup uzivatele, potrebujeme jednu radku
    // navic, protoze prikazova radka bude schovana a ostatni radky konzole
    // se budou vypisovat od pozice 0 a ne od pozice 1

    lineBuffer = new string[noOfLines];
    if(!lineBuffer)
    {
        destroy();
        return false;
    }

    commandLine = "_";

    return true;
}

//-----------------------------------------------------------------------------
// vypis textu do konzole
//-----------------------------------------------------------------------------

void AConsole::print(const char *str, ...)
{
    char buf[256];

    va_list list;

    if(str == NULL)
        return;

    va_start(list, str);
    vsprintf(buf, str, list);
    va_end(list);

    // pokud text bude delsi nez se vejde na radek, bude zkracen
    if (strlen(buf) > (unsigned int) maxLineChars)
    {
        buf[maxLineChars-1] = '\0';
    }

    // posuneme vsechny radky o jeden nahoru
    for(int p=noOfLines-1; p>0; p--)
    {
        lineBuffer[p] = lineBuffer[p-1];
    }

    string foo(buf);
    lineBuffer[0] = foo;
}

//-----------------------------------------------------------------------------
// vykresluje konzoli
//-----------------------------------------------------------------------------

void AConsole::render()
{
    if(!visible) return;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    renderBackground();

    unsigned char difference;
    unsigned char alpha = 255;

    // pokud je zapnut efekt slabnuti, zjistime po jakych castech budeme
    // zeslabovat
    if(fadeOut)
        difference = (unsigned char) (255 / noOfLines);


    glColor4ub(r, g, b, alpha);

    // pokud je povolen vstup uzivatele, zobrazujeme prikazovou radku
    if(userInput)
    {
        // vypis prikazove radky
        char buf[256];
        strcpy(buf, commandLine.c_str());
        text2D->print(posX+border, posY+border, buf);
    }

    // pocatecni pozice pro vypis radku
    int y = posY+border;

    // pokud je povolen vstup uzivatele, nejspodnejsi radka bude
    // prikazova radka, kam uzivatel pise prikazy
    if(userInput)
        y += text2D->getSizeHeight();

    // vypis radku od spoda nahoru

    // pokud je zapnut vstup uzivatele, vypisuje se o jednu radku konzole
    // mene, protoze se jako nulta radka vypisuje radka prikazova pro
    // vstup uzivatele
    int foo=0;
    if(userInput)
        foo=1;

    for(int p=0; p<noOfLines-foo; p++)
    {
        char buf[256];
        strcpy(buf, lineBuffer[p].c_str());

        // pokud je zapnut efekt zeslabnuti, zeslabujeme...
        if(fadeOut)
            alpha-=difference;

        glColor4ub(r, g, b, alpha);
        text2D->print(posX+border, y, buf);

        // posun vypisu radku nahoru
        y += text2D->getSizeHeight();
    }
}

//-----------------------------------------------------------------------------
// pise znad do prikazove radky
//-----------------------------------------------------------------------------

void AConsole::input(char c)
{
    // funguje pouze tehdy, je-li povolen vstup uzivatele
    if(userInput)
    {

    // vstup znaku
    if(isprint(c))
    {

        // pokud se na radku nic nevejde, nemuzeme zapsat znak
        if(commandLine.length()+1 >= (unsigned int) maxLineChars)
            return;

        char buf[256];
        strcpy(buf, commandLine.c_str());
        buf[strlen(buf)-1] = '\0';
        string foo(buf);
        foo += c;
        foo += "_";
        commandLine = foo;
    }

    // backspace maze jeden znak
    if(c == '\b')
    {
        // nemame co mazat
        if(commandLine.length() == 1)
            return;

        char buf[256];
        strcpy(buf, commandLine.c_str());
        buf[strlen(buf)-2] = '\0';
        string foo(buf);
        foo += "_";
        commandLine = foo;
    }
    // pokud stiskneme enter, potvrdime zadany prikaz
    if(c == '\r')
    {
        char buf[256];
        strcpy(buf, commandLine.c_str());
        buf[strlen(buf)-1] = '\0';

        print(buf);

        //zavolame funkci pro zpracovani prikazu
        if(func)
            func(buf);

        commandLine = "_";
    }

    } // if(userInput)
}

//-----------------------------------------------------------------------------
// vyprazdni konzoli
//-----------------------------------------------------------------------------

void AConsole::clear()
{
    for(int p=0; p<noOfLines; p++)
    {
        lineBuffer[p] = "";
        commandLine = "_";
    }
}

//-----------------------------------------------------------------------------
// prichozi udalost stisknuti klavesy z okna, ke kteremu je konzole asociovana
//-----------------------------------------------------------------------------

void AConsole::keyDownInput(SDL_keysym *keysym)
{
    // pokud je konzole viditelna
    if(visible)
    {
        if(keysym->sym == consoleOnOffKey)
        {
            hide();
            return;
        }

        // pokud je povolen vstup uzivatele
        if(userInput)
            input(keysym->sym);
    }
}

//-----------------------------------------------------------------------------
// nacte texturu pozadi konzole
//-----------------------------------------------------------------------------

bool AConsole::backgroundTexture(char *filename)
{
    if(loadTexture(filename, &texture))
    {
        isTextured = true;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// vykresluje pozadi konzole
//-----------------------------------------------------------------------------

void AConsole::renderBackground()
{
    if(!isTextured)
        return;

    glColor4ub(255, 255, 255, backgroundAlpha);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glBindTexture(GL_TEXTURE_2D, texture);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,text2D->getWindowWidth(),0,text2D->getWindowHeight(),-100,100);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);

        glTexCoord2f(0.0, 1.0);
        glVertex2d(posX, posY);

        glTexCoord2f(1.0, 1.0);
        glVertex2d(posX+width, posY);

        glTexCoord2f(1.0, 0.0);
        glVertex2d(posX+width, posY+height);

        glTexCoord2f(0.0, 0.0);
        glVertex2d(posX, posY+height);

    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

} // namespace astral3d
