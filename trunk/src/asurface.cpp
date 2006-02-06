 /*****************************************************************************
 * Astral3D is a 3D engine based on OpenGL and SDL.                           *
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

#include "asurface.h"

using namespace std;
namespace astral3d {

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

ASurface::ASurface()
{
    alpha = 255;
    window = NULL;
    textureID = 0;
    textureWidth = 0;
    textureHeight = 0;

    sourceBlendFactor = GL_SRC_ALPHA;
    destinationBlendFactor = GL_ONE_MINUS_SRC_ALPHA;
}

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

ASurface::ASurface(char *imageFileName, AWindow *window, unsigned int alpha)
{
    loadImage(imageFileName);
    setWindow(window);
    setAlpha(alpha);
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

ASurface::~ASurface()
{
    destroy();
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------

void ASurface::destroy()
{
    deleteTexture(&textureID);
}

//-----------------------------------------------------------------------------
// loadImage
//-----------------------------------------------------------------------------

bool ASurface::loadImage(char *imageFileName)
{
    if(!loadTexture(imageFileName, &textureID))
        return false;

    SDL_Surface *image = NULL;
    if(!(image = IMG_Load(imageFileName)))
        return false;

    textureWidth = image->w;
    textureHeight = image->h;

    SDL_FreeSurface(image);

    return true;
}

//-----------------------------------------------------------------------------
// setWindow
//-----------------------------------------------------------------------------

void ASurface::setWindow(AWindow *window)
{
    this->window = window;
}

//-----------------------------------------------------------------------------
// setAlpha
//-----------------------------------------------------------------------------

void ASurface::setAlpha(unsigned int alpha)
{
    this->alpha = 255 - alpha;
}

//-----------------------------------------------------------------------------
// begin
//-----------------------------------------------------------------------------

void ASurface::begin()
{
    glColor4ub(255, 255, 255, alpha);
    glEnable(GL_BLEND);
    glBlendFunc(sourceBlendFactor, destinationBlendFactor);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,window->getWidth(),0,window->getHeight(),-100,100);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
}

//-----------------------------------------------------------------------------
// end
//-----------------------------------------------------------------------------

void ASurface::end()
{
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------
// draw
//-----------------------------------------------------------------------------

void ASurface::draw(int x, int y, ARectangle source)
{
    draw(ARectangle(x, y, source.width, source.height), source);
}

//-----------------------------------------------------------------------------
// draw
//-----------------------------------------------------------------------------

void ASurface::draw(ARectangle target, ARectangle source)
{
    float x1 = (float) source.x / (float) textureWidth;
    float y1 = (float) source.y / (float) textureHeight;
    float x2 = (float) (source.x + source.width) / (float) textureWidth;
    float y2 = (float) (source.y + source.height) / (float) textureHeight;

    glTexCoord2f(x1, y2);
    glVertex2d(target.x, target.y + target.height);

    glTexCoord2f(x2, y2);
    glVertex2d(target.x + target.width, target.y + target.height);

    glTexCoord2f(x2, y1);
    glVertex2d(target.x + target.width, target.y);

    glTexCoord2f(x1, y1 );
    glVertex2d(target.x, target.y);
}

//-----------------------------------------------------------------------------
// blend function
//-----------------------------------------------------------------------------

void ASurface::setBlendFunc(GLenum sFactor, GLenum dFactor)
{
    sourceBlendFactor = sFactor;
    destinationBlendFactor = dFactor;
}

} // namespace astral3d
