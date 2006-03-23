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
 * @file aabstract.h Abstract classes.
 */
#ifndef AABSTRACT_H
#define AABSTRACT_H

#include <iostream>
#include "acollision.h"
#include "avector.h"
#include "a3ds.h"
#include "aexceptions.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

/**
 * Abstract class for loading and displaying 3D models.
 * This is abstract class for loading and displaying 3D models. See
 * A3DSModel class if you want to load and display 3D Studio (3DS) files.
 */
class Model3D
{
    public:
        virtual ~Model3D() {}
        virtual Model3D *load(char* filename, char *texturePath) = 0;
        virtual void render() = 0;
        virtual void destroy() = 0;
        virtual std::string getTexturePath() = 0;
        virtual A3DModel *get3DModel() = 0;
};

/**
 * Abstract class for loading and displaying levels.
 * This is abstract class for loading and displaying levels. See
 * ALevel class if you want to load and display Astral3D levels.
 */
class Level
{
    protected:
        // collision structure
        ACollisionPacket colPackage;

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

    public:
        virtual ~Level() {}

        virtual Level *load(char *filename, char *texturePath) = 0;

        /**
         * Saves the level.
         * This method saves the level to the file.
         * @param filename Filename of the new file with the level
         * @return 'true' if the save is successful
         * @see load
         */
        virtual void save(char *filename) = 0;

        /**
         * Draws the level.
         * This method draws the level on the screen.
         */
        virtual void render() = 0;

        /**
         * Destroys the level.
         * This method destroys the level and frees the memory.
         * @see load
         */
        virtual void destroy() = 0;

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
        virtual AVector getPosition(const AVector &pos, const AVector &vel) = 0;

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
        virtual AVector getDirection(const AVector &pos, const AVector &vel) = 0;

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
        virtual AVector getGravityDirection(const AVector &pos) = 0;

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
        virtual AVector getGravityPosition(const AVector &pos) = 0;

        /**
         * Builds the level from the 3DS model (A3DSModel).
         * This method builds the level from 3DS model (A3DSModel). If the
         * level is built the model isn't needed anymore.
         * @param model A3DSModel model
         * @return 'true' of the level is built successfuly
         */
        virtual Level *buildFromModel(Model3D *model) = 0;

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
        void setEllipsoid(const AVector &eRadius) { this->colPackage.eRadius = eRadius; }

        /**
         * Sets the level gravity.
         * This method sets the level gravity.
         * @param gravity Vector (AVector) describing the level gravity
         * @see getGravityPosition
         * @see getGravityDirection
         */
        void setGravity(const AVector &gravity) { gravityVector = gravity; }

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
};

/**
 * Abstract class for collision detection.
 */
class CollisionEllipsoid
{
    protected:
        Level *level;
        AVector eRadius;

    public:
        virtual ~CollisionEllipsoid() {}

        void setEllipsoid(const AVector &eRadius) { this->eRadius = eRadius; }
        void setLevel(Level *level)
        {
            if (!level)
            {
                throw ANullPointerException("void CollisionEllipsoid::setLevel(Level *level)");
            }
            this->level = level;
        }

        Level *getLevel() { return level; }

        virtual void checkLevelGravity() = 0;
};

} // namespace astral3d

#endif    // #ifndef AABSTRACT_H
