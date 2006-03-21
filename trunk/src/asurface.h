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
 * Struct defining a rectangle.
 * This struct defines a rectangle.
 */
struct ARectangle
{
    int x;
    int y;
    int width;
    int height;

    /**
     * Constructor.
     * Constructor.
     * @param x X-position
     * @param y Y-position
     * @param width Width of the rectaangle
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
         * Constructor.
         */
        ASurface();

        /**
         * Constructor.
         * Build constructor.
         * @param imageFilename Filename of the image (BMP, TGA, PNG, JPEG)
         * @param window Window which the surface will be associated with
         * @param alpha Transparency of the console (0 to 255 where 255 means full transparent)
         */
        ASurface(char *imageFileName, AWindow *window, unsigned int alpha = 0);

        /**
         * Destructor.
         * Destructor.
         */
        ~ASurface();

        /**
         * Destroys the surface.
         * It is called automatically from the destructor.
         */
        void destroy();

        /**
         * Loads the image.
         * This method loads the image of the surface.
         * @param imageFilename Image to load (BMP, TGA, PNG, JPEG)
         * @return 'true' if the image is loaded successfuly
         */
        bool loadImage(char *imageFileName);

        /**
         * Associates the surface with the window.
         * This method associates the surface with the window.
         * @param window Window to associate the surface with
         */
        void setWindow(AWindow *window);

        /**
         * Sets the transparency.
         * this method sets the transparency.
         * @param alpha Transparency of the console (0 to 255 where 255 means full transparent)
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
        void setBlendFunc(GLenum sFactor, GLenum dFactor);

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
         * @param x X-position of the image in the window from the left
         * @param y Y-position of the image in the window from the bottom
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
         * @return Window the surface is associated with
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
