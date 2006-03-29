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
 * @file atext.h AText2D class.
 */
#ifndef ATEXT_H
#define ATEXT_H

#ifdef WIN32
    #include <windows.h>
    #include <SDL.h>
#else
    #include "SDL.h"
#endif

#include <GL/gl.h>
#include <string>
#include <iostream>
#include <cstdarg>
#include <fstream>
#include <cstring>
#include <cctype>
#include <sstream>

#include "atexture.h"
#include "aerror.h"
#include "aexceptions.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

//-----------------------------------------------------------------------------
//  trida AText2D
//-----------------------------------------------------------------------------

/**
 * Class for text output.
 * This class handles text output.
 */
class AText2D
{
    private:
        GLuint texture;         // font texture
        int windowWidth;        // window width
        int windowHeight;       // window height
        int sizeWidth;          // character width when drawing
        int sizeHeight;         // character height when drawing
        int translate;          // translate
        float width;            // character width in the texture
        float height;           // character height in the texture
        int charsInLine;        // number of characters in o row
        std::string strFont;    // string describing a bitmap font

        void print(GLint x, GLint y, int sizeW, int sizeH, const char *str, ...);

    public:

        /**
         * Constructor.
         */
        AText2D() {}

        /**
         * Destructor.
         * Calls AText2D::destroy method.
         */
        ~AText2D() { destroy(); }

        /**
         * Constructor.
         * @param filename Texture filename (BMP, TGA, JPG and PNG files formats are supported)
         * @param translate Text translation
         * @param windowW Width of the main window
         * @param WindowH Height of the main window
         * @param fontW Width of the image with the font
         * @param fontH Height of the image with the font
         * @param charW Width of the character in the image
         * @param charH Height of the character in the image
         * @throw ATextureException
         */
        AText2D(char *filename, int translate, int windowW, int windowH, int fontW, int fontH, int charW, int charH);

        /**
         * Builds the font.
         * This method loads the texture and builds the font from it.
         * @param filename Texture filename (BMP, TGA, JPG and PNG files formats are supported)
         * @param translate Text translation
         * @param windowW Width of the main window
         * @param WindowH Height of the main window
         * @param fontW Width of the image with the font
         * @param fontH Height of the image with the font
         * @param charW Width of the character in the image
         * @param charH Height of the character in the image
         * @return Pointer to this instance
         * @see destroyFont
         * @throw ATextureException
         */
        AText2D *build(char *filename, int translate, int windowW, int windowH, int fontW, int fontH, int charW, int charH);
        /**
         * Sets the string describing the font.
         * This method sets the text description of the font. The text is read
         * from left top corner.
         * @param str Text description of the font
         * @see loadString
         */
        void setString(const std::string &str) { this->strFont = str; }
        /**
         * Loads font description from the file.
         * This method sets the text description of the font. String describing
         * the font is loaded from the file.
         * @param filename File with the font description
         * @return String describing the font
         * @see setString
         * @throw AReadFileException
         */
        std::string loadString(char *filename);
        /**
         * Sets the output size of characters.
         * This method sets the output size of characters and the size
         * of the text translation.
         * @param sizeW Width of the character
         * @param sizeH Height of the character
         * @param translate Translation
         */
 inline void setSize(int sizeW, int sizeH, int translate);
        /**
         * Prints the output.
         * This method prints the text output. This method should be called
         * when rendering the scene.
         * @param x x-position of the text from the left (in pixels)
         * @param y y-position of the text from the bottom (in pixels)
         * @param str String to be printed using 'printf' function format
         */
        void print(GLint x, GLint y, const char *str, ...);

        /**
         * Returns translation.
         * @return Translation
         */
        int getTranslate()     { return translate; }
        /**
         * Returns output width of the characters.
         * @return Output width of the characters
         */
        int getSizeWidth()     { return sizeWidth; }
        /**
         * Returns output height of the characters.
         * @return Output height of the characters
         */
        int getSizeHeight()    { return sizeHeight; }
        /**
         * Returns width of the main window.
         * @return Width of the main window.
         */
        int getWindowWidth()   { return windowWidth; }
        /**
         * Returns height of the main window.
         * @return Height of the main window.
         */
        int getWindowHeight()  { return windowHeight; }

        /**
         * Destroys the font.
         * This method frees the memory and destroys the font.
         * It is called automatically from the destructor.
         * @see buildFont
         */
        inline void destroy();
};

//-----------------------------------------------------------------------------
//  inline functions
//-----------------------------------------------------------------------------

void AText2D::setSize(int sizeW, int sizeH, int translate)
{
    this->sizeWidth = sizeW;
    this->sizeHeight = sizeH;
    this->translate = translate;
}

void AText2D::destroy()
{
    deleteTexture(&(this->texture));
}

} // namespace astral3d

#endif    // #ifndef ATEXT_H
