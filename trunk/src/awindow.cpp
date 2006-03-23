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

#include "awindow.h"

using namespace std;
namespace astral3d {

// funkce pro ukonceni aplikace
void appExit(int exitCode)
{
    exit(exitCode);
}

AWindow::AWindow(int width, int height, int bpp, bool resizable, bool fullscreen)
{
    create(width, height, bpp, resizable, fullscreen);
}

//-----------------------------------------------------------------------------
// vykresleni sceny
//-----------------------------------------------------------------------------

void AWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//-----------------------------------------------------------------------------
// nastavi titulek okna
//-----------------------------------------------------------------------------

void AWindow::setCaption(char *caption)
{
    SDL_WM_SetCaption(caption, caption);
}

//-----------------------------------------------------------------------------
// nastavi ikonu okna
//-----------------------------------------------------------------------------

void AWindow::setIcon(char *filename)
{
    SDL_WM_SetIcon(SDL_LoadBMP(filename), NULL);
}

//-----------------------------------------------------------------------------
// uvolneni pameti
//-----------------------------------------------------------------------------

void AWindow::destroy(int retVal)
{
    this->exit();
    if(screen)
    {
        SDL_FreeSurface(screen);
    }
    SDL_Quit();
    running = false;
    appExit(retVal);
}

//-----------------------------------------------------------------------------
// vytvori nove okno aplikace a provede potrebna nastaveni
//-----------------------------------------------------------------------------

AWindow *AWindow::create(int width, int height, int bpp, bool resizable, bool fullscreen)
{
    // nastaveni promennych
    this->width = width;
    this->height = height;
    this->bpp = bpp;

    // inicializace videomodu
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        stringstream foo;
        foo << "AWindow::create("<<width<<", "<<height<<", "<<bpp<<", "<<resizable<<", "<<fullscreen<<")";
        setAstral3DError(SDL_GetError(), foo.str(), "SDL_Init(SDL_INIT_VIDEO)");

        throw ASDLException("AWindow *AWindow::create(int width, int height, int bpp, bool resizable, bool fullscreen)");
    }

    // nastaveni atributu zobrazeni
    this->flags = SDL_OPENGL;
    this->flags |= SDL_GL_DOUBLEBUFFER;
    this->flags |= SDL_HWPALETTE;

    // pokud mame povoleno menit velikost okna
    if(resizable)
        this->flags |= SDL_RESIZABLE;

    // ziskani informaci o soucasnem zobrazeni
    const SDL_VideoInfo *video_info = SDL_GetVideoInfo();

    if(video_info == NULL)
    {
        stringstream foo;
        foo << "AWindow::create("<<width<<", "<<height<<", "<<bpp<<", "<<resizable<<", "<<fullscreen<<")";
        setAstral3DError(SDL_GetError(), foo.str(), "SDL_GetVideoInfo()");

        throw ASDLException("AWindow *AWindow::create(int width, int height, int bpp, bool resizable, bool fullscreen)");
    }

    // nastaveni dalsich parametru zobrazeni podle ziskanych informaci
    if(video_info->hw_available)
        this->flags |= SDL_HWSURFACE;
    else
        this->flags |= SDL_SWSURFACE;

    if(video_info->blit_hw)
        this->flags |= SDL_HWACCEL;

    // nastaveni doublebufferingu pro OpenGL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, this->bpp);

    // vlastni vytvoreni okna
    this->fullscreen = fullscreen;

    int final_flags = flags;
    if(this->fullscreen)
    {
        final_flags |= SDL_FULLSCREEN;
    }

    this->screen = SDL_SetVideoMode(this->width, this->height,
                                    this->bpp, final_flags);

    if(screen == NULL)
    {
        stringstream foo;
        stringstream bar;
        foo << "AWindow::create("<<width<<", "<<height<<", "<<bpp<<", "<<resizable<<", "<<fullscreen<<")";
        bar << "SDL_SetVideoMode("<<width<<", "<<height<<", "<<bpp<<", "<<final_flags<<")";
        setAstral3DError(SDL_GetError(), foo.str(), bar.str());

        throw ASDLException("AWindow *AWindow::create(int width, int height, int bpp, bool resizable, bool fullscreen)");
    }

    // inicializuje OpenGL okno
    this->initGL();

    // a nastavi potrebne parametry
    this->resizeScreen(this->width, this->height);

    // uzivatelova inicializace
    if(!this->init())
    {
        this->exit();
        if(screen) SDL_FreeSurface(screen);
        SDL_Quit();

        throw AException("AWindow *AWindow::create(int width, int height, int bpp, bool resizable, bool fullscreen)");
    }

    glLoadIdentity();

    return this;
}

//-----------------------------------------------------------------------------
// inicializuje rozhrani OpenGL
//-----------------------------------------------------------------------------

void AWindow::initGL()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

//-----------------------------------------------------------------------------
// nastavi vlastnosti zobrazeni na zaklade velikosti okna
//-----------------------------------------------------------------------------

void AWindow::resizeScreen(int width, int height)
{
    // nesmime delit nulou pri vypocitavani aspektu pomeru
    if (height==0)
        height=1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f , 5000.0f);

    glMatrixMode(GL_MODELVIEW);
}

//-----------------------------------------------------------------------------
// zmeni fullscreen mod
//-----------------------------------------------------------------------------

void AWindow::toggleFullscreen(bool fullscreen)
{
    this->fullscreen = fullscreen;

    int final_flags = this->flags;
    if(this->fullscreen)
        final_flags |= SDL_FULLSCREEN;

    SDL_FreeSurface(this->screen);

    this->screen = SDL_SetVideoMode(this->width, this->height,
                                    this->bpp, final_flags);

    this->resizeScreen(this->width, this->height);

    if(this->screen == NULL)
    {
        stringstream foo;
        stringstream bar;
        foo << "AWindow::toggleFullscreen("<<fullscreen<<")";
        bar << "SDL_SetVideoMode("<<width<<", "<<height<<", "<<bpp<<", "<<final_flags<<")";
        setAstral3DError(SDL_GetError(), foo.str(), bar.str());

        cerr << getAstral3DError();

        this->destroy(1);
    }
}

//-----------------------------------------------------------------------------
// hlavni smycka okna
//-----------------------------------------------------------------------------

void AWindow::run()
{
    running = true;

    int final_flags;

    SDL_Event event;

    while(running)
    {
        // zpracovani udalosti
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                // reakce na udalost ukonceni programu
                case SDL_QUIT:
                    this->destroy(0);
                    running = false;
                    break;

                // reakce na udalost stiskuti klavesy
                case SDL_KEYDOWN:
                    // pokud ma okno asociovanou konzoli, zobrazi ji,
                    // pokud je stisknuta klavesa pro zobrazeni konzole
                    if(this->console)
                    {
                        // pokud je konzole skryta a stiskne se klavesa
                        // pro jeji zobrazeni, konzole je zobrazena
                        if(!console->isVisible())
                        {
                            if(event.key.keysym.sym == console->getConsoleOnOffKey())
                                console->show();
                        }
                        else
                        {
                            // jinak predame vstup do konzole
                            console->keyDownInput(&event.key.keysym);
                        }
                    }
                    // do metody okna keyDown predame informace o stisknute
                    // klavese
                    this->keyDown(&event.key.keysym);
                    break;

                // reakce na udalost uvolneni klavesy
                case SDL_KEYUP:
                    this->keyUp(&event.key.keysym);
                    break;

                // reakce na udalost pohybu mysi
                case SDL_MOUSEMOTION:
                    this->mouseX = event.motion.x;
                    this->mouseY = event.motion.y;
                    break;

                // reakce na stisk tlacitka mysi
                case SDL_MOUSEBUTTONDOWN:
                    this->mouseButtonDown(event.button.x, event.button.y, event.button.button);
                    break;

                // reakce na uvolneni tlacitka mysi
                case SDL_MOUSEBUTTONUP:
                    this->mouseButtonUp(event.button.x, event.button.y, event.button.button);
                    break;

                // reakce na zmenu velikosti okna
                case SDL_VIDEORESIZE:
                    final_flags = flags;
                    if(this->fullscreen)
                        final_flags |= SDL_FULLSCREEN;
                    this->screen = SDL_SetVideoMode(event.resize.w,
                                                    event.resize.h,
                                                    this->bpp, final_flags);

                    this->resizeScreen(event.resize.w, event.resize.h);

                    if(this->screen == NULL)
                    {
                        stringstream foo;
                        stringstream bar;
                        foo << "AWindow::run()";
                        bar << "SDL_SetVideoMode("<<event.resize.w<<", "<<event.resize.h<<", "<<this->bpp<<", "<<final_flags<<")";
                        setAstral3DError(SDL_GetError(), foo.str(), bar.str());

                        cerr << getAstral3DError();

                        this->destroy(1);
                    }
                    break;

                default:
                    break;
            }
        }

        this->loop();

        // vykresleni sceny
        this->render();
        // prohozeni bufferu
        SDL_GL_SwapBuffers();
    }
}

} // namespace astral3d
