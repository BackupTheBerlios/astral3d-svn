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

/******************************************************************************
 N O T E:

 Written thanks the great project APRON TUTORIALS at http://www.morrowland.com

******************************************************************************/

/**
 * @file a3dsmodel.h A3DSModel class.
 */
#ifndef A3DSMODEL_H
#define A3DSMODEL_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <cmath>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>

#include "atexture.h"
#include "a3ds.h"
#include "aerror.h"

#ifdef WIN32
    #include <SDL.h>
#else
    #include "SDL.h"
#endif

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

#define MAXTEXTURE 100

/**
 * Class for loading and rendering 3DS models.
 * This class is the interface for loading and displaying 3DS file formats.
 */
class A3DSModel
{
  private:

    A3DModel *m3DModel;                   // 3ds model
    A3DSLoader *mLoad3ds;                 // 3ds loader
    GLuint TextureArray3ds[MAXTEXTURE];   // textures for 3ds model
    std::string texturePath;              // texture path directory

  public:
    /**
     * Constructor.
     * Constructor.
     */
    A3DSModel() { m3DModel = NULL; mLoad3ds = NULL; }

    /**
     * Destructor.
     * Destructor calls A3DSModel::destroy method.
     */
    ~A3DSModel() { destroy(); }

    /**
     * Loads 3DS model.
     * This method loads 3DS model from the file.
     * @param filename Filename of the 3DS file
     * @param texturePath Path to the directory where texture files are located
     * @return 'true' if the 3DS file is loaded successfuly
     * @see destroy
     */
    bool load(char* filename, char *texturePath);
    /**
     * Draws the model.
     * This method draws loaded model to the screen. Model has to be loaded
     * first.
     * @see load
     */
    void render();
    /**
     * Destroys the model.
     * This method frees the memory and destroys the model. This method is called
     * automatically from the destructor.
     */
    void destroy();
    /**
     * Returns 3D model containing model data.
     * This method returns model (A3DModel) structure containing model data.
     * @return pointer to A3DModel structure
     */
    A3DModel *get3DModel() { return m3DModel; }

    /**
     * Returns texture path directory.
     * This method returns the path to the directory where textures are located.
     * @return Texture path directory
     * @see load
     */
    std::string getTexturePath() { return texturePath; }
};

} // namespace astral3d

#endif    // #ifndef A3DSMODEL_H
