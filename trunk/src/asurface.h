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

/**
 * @file asurface.h ASurface class.
 */
#ifndef ASURFACE_H
#define ASURFACE_H

#include <GL/gl.h>
#include "awindow.h"

#ifdef WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include "SDL.h"
#include "SDL_image.h"
#endif

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

/**
 * Structure describing a rectangle.
 */
struct ARectangle
{
    int x;
    int y;
    int width;
    int height;

    /**
     * Constructor.
     * @param x x-position
     * @param y y-position
     * @param width Width of the rectangle
     * @param height Height of the rectangle
     */
    inline ARectangle(int x = 0, int y = 0, int width = 0, int height = 0);
};

/**
 * Class for displaying 2D images.
 * This class can display 2D images in 3D space.
 */
class ASurface
{
    private:

        GLuint  textureID;
        int     textureWidth;
        int     textureHeight;

        GLenum sourceBlendFactor;
        GLenum destinationBlendFactor;

        unsigned char alpha;
        AWindow *window;

    public:

        /**
         * Constructor.
         */
        ASurface();

        /**
         * Constructor.
         * @param imageFilename Filename of the image (BMP, TGA, PNG, JPEG)
         * @param window Window the surface is associated to
         * @param alpha Transparency of the console (0 to 255 where 255 is full transparent)
         * @throw ATextureException
         * @throw ANullPointerException
         */
        ASurface(char *imageFileName, AWindow *window, unsigned int alpha = 0);

        /**
         * Destructor.
         * Calls ASurface::destroy method.
         */
        ~ASurface();

        /**
         * Destroys the surface.
         * It is called automatically from the destructor.
         */
        void destroy();

        /**
         * Loads the image.
         * This method loads the image for the surface.
         * @param imageFilename Image to load (BMP, TGA, PNG, JPEG)
         * @throw ATextureException
         */
        void loadImage(char *imageFileName);

        /**
         * Sets the window.
         * Sets the window for the surface.
         * @param window Window to associate the surface to
         * @throw ANullPointerException
         */
        void setWindow(AWindow *window);

        /**
         * Sets the transparency.
         * This method sets the transparency.
         * @param alpha Transparency of the console (0 to 255 where 255 is full transparent)
         */
        void setAlpha(unsigned int alpha);

        /**
         * Sets blending function.
         * This method sets parameters for blending.
         * @param sFactor Source factor of the blending
         * <ul>
         *  <li>GL_ZERO</li>
         *  <li>GL_ONE</li>
         *  <li>GL_DST_COLOR</li>
         *  <li>GL_ONE_MINUS_DST_COLOR</li>
         *  <li>GL_SRC_ALPHA</li>
         *  <li>GL_ONE_MINUS_SRC_COLOR</li>
         *  <li>GL_DST_ALPHA</li>
         *  <li>GL_ONE_MINUS_DST_ALPHA</li>
         *  <li>GL_SRC_ALPHA_SATURATE</li>
         * </ul>
         * @param dFactor Destination factor of the blending
         * <ul>
         *  <li>GL_ZERO</li>
         *  <li>GL_ONE</li>
         *  <li>GL_SRC_COLOR</li>
         *  <li>GL_ONE_MINUS_SRC_COLOR</li>
         *  <li>GL_SRC_ALPHA</li>
         *  <li>GL_ONE_MINUS_SRC_ALPHA</li>
         *  <li>GL_DST_ALPHA</li>
         *  <li>GL_ONE_MINUS_DST_ALPHA</li>
         * </ul>
         */
        void setBlending(GLenum sFactor, GLenum dFactor);

        /**
         * Begin drawing.
         * You must call ASurface::end when you finish drawing.
         * @see end
         * @see draw
         */
        void begin();

        /**
         * Ends drawing.
         * This method must be called when finishing drawing.
         * @see begin
         * @see draw
         */
        void end();

        /**
         * Draws the image.
         * You must call ASurface::begin before drawing.
         * @param x x-position of the image in the window from the left
         * @param y y-position of the image in the window from the bottom
         * @param source Rectangle to be drawn from the image (in pixels)
         * @see begin
         * @see end
         */
        void draw(int x, int y, ARectangle source);

        /**
         * Draws the image.
         * You must call ASurface::begin before drawing.
         * @param target Rectangle in the window to be drawn to (in pixels)
         * @param source Rectangle to be drawn from the image (in pixels)
         * @see begin
         * @see end
         */
        void draw(ARectangle target, ARectangle source);


        /**
         * Returns texture ID.
         * @return Texture ID
         */
        GLuint        getTextureID()      { return textureID; }
        /**
         * Returns texture width.
         * @return Texture width
         */
        int           getTextureWidth()   { return textureWidth; }
        /**
         * Returns texture height.
         * @return Texture height
         */
        int           getTextureHeight()  { return textureHeight; }
        /**
         * Returns transparency.
         * @return Alpha value
         */
        unsigned char getAlpha()          { return alpha;}
        /**
         * Returns associated window.
         * @return Window the surface is associated to
         */
        AWindow*      getWindow()         { return window; }
};

//-----------------------------------------------------------------------------
//
//  INLINE FUNCTIONS
//
//-----------------------------------------------------------------------------

ARectangle::ARectangle(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

} // namespace astral3d

#endif    // #ifndef AIMAGE_H
