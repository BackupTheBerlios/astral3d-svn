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
class ALevel
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

        // collision structure
        ACollisionPacket colPackage;

        // max depth of recursion when making collision detection
        int collisionRecursionDepth;

        // gravity
        AVector gravityVector;

        // center of the sphere which we do collision detection tests in
        AVector spherePosition;

        // radius of this sphere
        double sphereRadius;

        // enables collision detection only in the given sphere
        bool sphere;

        // enables gravity
        bool gravity;

        // create lists of triangles according to the textures
        bool createLists();

    private:

        AVector collideWithWorld(const AVector &pos, const AVector &vel);

        void checkCollision();

    public:
        /**
         * Constructor.
         * Constructor.
         */
        ALevel();

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
         * @return 'true' if the level is successfuly loaded
         * @see destroy
         */
        bool load(char *filename, char *texturePath);

        /**
         * Saves the level.
         * This method saves the level to the file.
         * @param filename Filename of the new file with the level
         * @return 'true' if the save is successful
         * @see load
         */
        bool save(char *filename);

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
         * Sets the collision ellipsoid.
         * This method sets the collision ellipsoid for the level. Collision
         * ellipsoid is the structure that the collision detection is done
         * against.
         * @param eRadius Vector (AVector) describing the ellipsoid. X, Y and Z
         *                values of the vector characterize radiuses of the
         *                ellipsoid. X and Z values should be equal but Y should be
         *                bigger - than the ellipsoid will look like a human body.
         *                If X=Y=Z than we get a collision sphere.
         */
        inline void setEllipsoid(const AVector &eRadius);

        /**
         * Sets the level gravity.
         * This method sets the level gravity.
         * @param gravity Vector (AVector) describing the level gravity
         * @see getGravityPosition
         * @see getGravityDirection
         */
        inline void setGravity(const AVector &gravity);

        /**
         * Enables gravity.
         * This method enables gravity in the level.
         */
        void enableGravity() { this->gravity = true; }

        /**
         * Disables gravity.
         * This method disables gravity in the level.
         */
        void disableGravity() { this->gravity = false; }

        /**
         * Returns 'true' if the gravity is enabled.
         * This method returns 'true' if the gravity is enabled.
         * @return 'true' if the gravity is enabled
         * @see enableGravity()
         * @see disableGravity()
         * @see setGravity()
         */
        bool isGravityEnabled() { return this->gravity; }

        /**
         * Enables collision detection sphere test.
         * This method enables collision detection sphere test. It means
         * that the collision detection is tested only against the triangles
         * being in the give sphere. This sphere is setup by ALevel::setSpherePosition
         * and ALevel::setSphereRadius methods. Collision detection is very expensive
         * calculation so the sphere test saves time.
         * @see disableSphere
         * @see setSpherePosition
         * @see setSphereRadius
         */
        void enableSphere() { this->sphere = true; }

        /**
         * Disables collision detection sphere test.
         * This method disables collision detection sphere test.
         * See ALevel::enableSphere for more info.
         * @see enableSphere
         */
        void disableSphere() { this->sphere = false; }

        /**
         * Sets center of the sphere for collision detection sphere test.
         * This method sets center of the sphere for collision detection sphere test.
         * See ALevel::enableSphere for more info.
         * @param sPosition Center of the sphere for collision detection sphere test
         * @see enableSphere
         * @see disablesphere
         * @see setSphereRadius
         */
        void setSpherePosition(const AVector &sPosition) { this->spherePosition = sPosition; }

        /**
         * Sets radius of the sphere for collision detection sphere test.
         * This method sets radius of the sphere for collision detection sphere test.
         * See ALevel::enableSphere for more info.
         * @param sRadius Radius of the sphere for collision detection sphere test
         * @see enableSphere
         * @see disablesphere
         * @see setSphereRadius
         */
        void setSphereRadius(double sRadius) { this->sphereRadius = sRadius; }

        /**
         * Saves lists of triangles according to the textures.
         * This method saves lists of triangles according to the textures.
         * Only for debugging needs.
         * @param filename Name of the file to save to
         */
        bool saveListOfTriangles(char *filename);

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
         * @return 'true' of the level is built successfuly
         */
        bool buildFromModel(A3DSModel *model);
};

//-----------------------------------------------------------------------------
//  sets the collision ellipsoid
//-----------------------------------------------------------------------------

void ALevel::setEllipsoid(const AVector &eRadius)
{
    this->colPackage.eRadius = eRadius;
}

//-----------------------------------------------------------------------------
//  sets the gravity
//-----------------------------------------------------------------------------

void ALevel::setGravity(const AVector &gravity)
{
    this->gravityVector = gravity;
}

//-----------------------------------------------------------------------------
//  destructor
//-----------------------------------------------------------------------------

ALevel::~ALevel()
{
    this->destroy();
}

} // namespace astral3d

#endif // #ifndef ALEVEL_H
