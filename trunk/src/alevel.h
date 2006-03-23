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
 * @file alevel.h ALevel class.
 */
#ifndef ALEVEL_H
#define ALEVEL_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

#include <GL/gl.h>

#include "atexture.h"
#include "avector.h"
#include "apolygons.h"
#include "acollision.h"
#include "a3dsmodel.h"
#include "aerror.h"
#include "aabstract.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

typedef GLuint* pGLuint;

//-----------------------------------------------------------------------------
//  ALevel class
//-----------------------------------------------------------------------------

/**
 * Class for loading and rendering levels.
 * This class is used for loading and rendering a level and provides
 * the basic interface for collision detection and response with the camera (ACamera
 * class).
 */
class ALevel : public Level
{
    private:
        ATriangle   *triangles;         // triangles building the level
        GLuint      *textures;          // level textures
        std::string *textureNames;      // file names of the textures

        GLuint numOfTriangles;          // number of triangles building the level
        GLuint numOfTextures;           // number of loaded textures

        // lists of triangles, each list contains list of triangles
        // having the same texture - this is used for speed up rendering
        GLuint **listOfTriangles;

        // number of triangles in each list
        GLuint *numberOfTrianglesInList;

        // max depth of recursion when testing collision detection
        int collisionRecursionDepth;

    private:

        // create lists of triangles according to the textures
        bool createLists();

        AVector collideWithWorld(const AVector &pos, const AVector &vel);

        void checkCollision();

    public:
        /**
         * Constructor.
         * Constructor.
         */
        ALevel();

        ALevel(char *filename, char *texturePath);

        ALevel(Model3D *model);

        /**
         * Destructor.
         * Calls Alevel::destroy method.
         *
         * @see destroy
         */
        inline ~ALevel();

        /**
         * Loads the level from the text file.
         * This method loads the level from the given text file.
         * @n
         * @n
         * Format of the text file containing the level:
         * @code
         * number_of_textures
         *
         * id_of_texture_1    texture_1_filename
         * id_of_texture_2    texture_2_filename
         * id_of_texture_3    texture_3_filename
         * ...
         *
         * number_of_triangles
         *
         * triangle_texture_id
         * vertex_1_x  vertex_1_y  vertex_1_z  vertex_1_u  vertex_1_v
         * vertex_2_x  vertex_2_y  vertex_2_z  vertex_2_u  vertex_2_v
         * vertex_3_x  vertex_3_y  vertex_3_z  vertex_3_u  vertex_3_v
         * normal_x  normal_y  normal_z
         *
         * ...
         * @endcode
         * @n
         * Example:
         * @n
         * @code
         * 2
         *
         * 0 floor.bmp
         * 1 wall.bmp
         *
         * 3
         *
         * 0
         * -5    0    5          0   5
         *  5    0    5          5   5
         *  5    0   -5          5   0
         *  0    1    0
         *
         * 1
         *  5    0   -5          5   0
         * -5    0   -5          0   0
         * -5    0    5          0   5
         *  0    1    0
         *
         * 1
         * -5    1.5    5        0   5
         *  5    1.5    5        5   5
         *  5    1.5   -5        5   0
         *  0     -1    0
         * @endcode
         *
         * @param *filename Filename of the file with the level
         * @param *texturePath Path to the directory with textures
         * @see destroy
         */
        ALevel *load(char *filename, char *texturePath);

        /**
         * Saves the level.
         * This method saves the level to the file.
         * @param filename Filename of the new file with the level
         * @see load
         */
        void save(char *filename);

        /**
         * Draws the level.
         * This method draws the level on the screen.
         */
        void render();

        /**
         * Destroys the level.
         * This method destroys the level and frees the memory.
         * @see load
         */
        void destroy();

        /**
         * Returns the position after the collision detection and response.
         * This metod returns new position (AVector) in 3D space according to
         * the requested move, collision detection and response.
         * @param pos Starting position of the move
         * @param vel Requested move
         * @return New position after the collision detection and response
         * @see setEllipsoid
         * @see getDirection
         */
        AVector getPosition(const AVector &pos, const AVector &vel);

        /**
         * Returns new vector of movement after the collision detection and response.
         * This metod returns new vector (AVector) of movement in 3D space according to
         * the requested move, collision detection and response.
         * @param pos Starting position of the move
         * @param vel Requested move
         * @return New vector of movement after the collision detection and response
         * @see setEllipsoid
         * @see getPosition
         */
        AVector getDirection(const AVector &pos, const AVector &vel);

        /**
         * Returns new vector of movement after the collision detection and response.
         * This metod returns new vector (AVector) of movement in 3D space according to
         * the gravity, collision detection and response.
         * @param pos Starting position of the move
         * @return New vector of movement after the collision detection and response
         * @see setEllipsoid
         * @see getDirection
         * @see getGravityPosition
         * @see setGravity
         */
        AVector getGravityDirection(const AVector &pos);

        /**
         * Returns the position after the collision detection and response.
         * This metod returns new position (AVector) in 3D space according to
         * the gravity, collision detection and response.
         * @param pos Starting position of the move
         * @return New position after the collision detection and response
         * @see setEllipsoid
         * @see getDirection
         * @see getGravityDirection
         * @see setGravity
         */
        AVector getGravityPosition(const AVector &pos);

        /**
         * Saves lists of triangles according to the textures.
         * This method saves lists of triangles according to the textures.
         * Only for debugging needs.
         * @param filename Name of the file to save to
         */
        void saveListOfTriangles(char *filename);

        /**
         * Adds the triangle (ATrianagle) to the level.
         * This method adds the new triangle to the level.
         * @param triangle New triangle to add
         * @see removeTriangle
         */
        bool addTriangle(ATriangle triangle);

        /**
         * Removes the triangle from the level.
         * This method removes the triangle from the level. Actually this
         * method only sets triangle validity to 'false' and removes it from
         * the list of triangles. Triangle is still in the memory but isn't
         * rendered and isn't saved when calling ALevel::save method.
         * @param id Triangles id in the buffer
         * @see addTriangle
         * @see save
         */
        bool removeTriangle(GLuint id);

        /**
         * Splits the triangles.
         * This method splits too 'big' triangles into smaller ones. Triangles
         * having the area bigger than the given parameter are split up into
         * two new smaller triangles. Old triangles are removed.
         * @param s Border area for splitting up the triangles
         * @param recursive Should the method be called recursively
         *                  (this causes that new triangles are also tested
         *                   and split up if necessary).
         * @see addTriangle
         * @see removeTriangle
         */
        void splitTriangles(double s, bool recursive=false);

        /**
         * Builds the level from the 3DS model (A3DSModel).
         * This method builds the level from 3DS model (A3DSModel). If the
         * level is built the model isn't needed anymore.
         * @param model A3DSModel model
         */
        ALevel *buildFromModel(Model3D *model);
};

//-----------------------------------------------------------------------------
//  destructor
//-----------------------------------------------------------------------------

ALevel::~ALevel()
{
    this->destroy();
}

} // namespace astral3d

#endif // #ifndef ALEVEL_H
