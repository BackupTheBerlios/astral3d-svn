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
 * @file atexture.h Functions for loading textures.
 */
#ifndef ATEXTURE_H
#define ATEXTURE_H

#include <cstring>
#include <cctype>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>

#include "aerror.h"

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

#define   BMP    1
#define   TGA    2
#define   JPG    3
#define   PNG    4
#define   NA     9

/**
 * Loads the texture.
 * This function loads and creates the texture from the file. File must be
 * BMP, PNG, TGA or JPEG image.
 * @param filename Image filename (BMP, TGA, PNG, JPEG)
 * @param texture Pointer to the texture identifier
 * @param min
 * @param mag
 * @return True if the texture is loaded and created successfuly
 */
bool loadTexture(char *filename, GLuint *texture, GLint min=GL_LINEAR, GLint mag=GL_LINEAR);
/**
 * Creates the texture.
 * This function creates the texture from SDL_Surface structure.
 * @param Image Pointer to the SDL_Surface structure
 * @param texture Pointer to the texture identifier
 * @param min
 * @param mag
 * @param type Type of the image in the surface to create the texture from (BMP, TGA, PNG, JPG)
 * @return True if the texture is created successfuly
 */
bool createTexture(SDL_Surface *Image, GLuint *texture, GLint min=GL_LINEAR, GLint mag=GL_LINEAR, int type=BMP);
/**
 * Loads the texture with MipMapping.
 * This function loads and creates the texture with MipMapping.
 * @param filename Image filename (BMP, TGA, PNG, JPEG)
 * @param texture Pointer to the texture identifier
 * @return True if the texture is loaded and created successfuly
 */
bool loadTextureMipMap(char *filename, GLuint *texture);
/**
 * Creates the texture with MipMapping.
 * This function creates the texture from SDL_Surface structure with MipMapping.
 * @param Image Pointer to the SDL_Surface structure
 * @param texture Pointer to the texture identifier
 * @param type Type of the image in the surface to create the texture from (BMP, TGA, PNG, JPG)
 * @return True if the texture is created successfuly
 */
bool createTextureMipMap(SDL_Surface *Image, GLuint *texture, int type=BMP);

/**
 * Frees the texture from the memory.
 * This function frees the texture from the memory.
 * @param texture Texture ID
 */
void deleteTexture(GLuint *texture);

} // namespace astral3d

#endif    // #ifndef ATEXTURE_H
