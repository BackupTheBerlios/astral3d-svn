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
        /**
         * Destructor.
         */
        virtual ~Model3D() {}
        /**
         * Loads 3D model from the file.
         * @param filename Filename of the 3D model
         * @param texturePath Path to the directory containing model textures
         * @return Pointer to this instance
         */
        virtual Model3D *load(char* filename, char *texturePath) = 0;
        /**
         * Renderes the model.
         */
        virtual void render() = 0;
        /**
         * Destroys the model.
         */
        virtual void destroy() = 0;
        /**
         * Returns the texture path.
         * @return Path to the directory containing model textures
         */
        virtual std::string getTexturePath() = 0;
        /**
         * Returns the texture path.
         * @return Path to the directory containing model textures
         */
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
        /**
         * Destructor.
         */
        virtual ~Level() {}

        /**
         * Loads the level.
         * This method loads the level from the file.
         * @param filename Filename of the level
         * @param texturePath Path to the directory containing level textures
         * @return Pointer to this instance
         */
        virtual Level *load(char *filename, char *texturePath) = 0;

        /**
         * Saves the level.
         * This method saves the level to the file.
         * @param filename Filename to save to
         * @see load
         */
        virtual void save(char *filename) = 0;

        /**
         * Renderes the level.
         */
        virtual void render() = 0;

        /**
         * Destroys the level.
         * @see load
         */
        virtual void destroy() = 0;

        /**
         * Returns new position after the collision detection and response.
         * This metod returns new position according to the requested move,
         * collision detection and response.
         * @param pos Starting position of the move
         * @param vel Requested move
         * @return New position
         * @see getDirection
         */
        virtual AVector getPosition(const AVector &pos, const AVector &vel) = 0;

        /**
         * Returns new vector of movement after the collision detection and response.
         * This metod returns new vector of movement according to the requested
         * move, collision detection and response.
         * @param pos Starting position of the move
         * @param vel Requested move
         * @return New vector of movement
         * @see getPosition
         */
        virtual AVector getDirection(const AVector &pos, const AVector &vel) = 0;

        /**
         * Returns new vector of movement after the collision detection and response.
         * This metod returns new vector of movement according to the gravity,
         * collision detection and response.
         * @param pos Starting position of the move
         * @return New vector of movement
         * @see getDirection
         * @see getGravityPosition
         * @see setGravity
         */
        virtual AVector getGravityDirection(const AVector &pos) = 0;

        /**
         * Returns new position after the collision detection and response.
         * This metod returns new position according to the gravity,
         * collision detection and response.
         * @param pos Starting position of the move
         * @return New position
         * @see getDirection
         * @see getGravityDirection
         * @see setGravity
         */
        virtual AVector getGravityPosition(const AVector &pos) = 0;

        /**
         * Builds the level from the 3D model.
         * This method builds the level from the 3D model. When the
         * level is built the model isn't needed anymore.
         * @param model Model3D representing 3D model
         * @return Pointer to this instance
         */
        virtual Level *buildFromModel(Model3D *model) = 0;

        /**
         * Sets the collision ellipsoid.
         * This method sets the collision ellipsoid for the level. Collision
         * ellipsoid is the shape that the collision detection is done
         * against.
         * @param eRadius Vector representing the ellipsoid. x, y and z
         *                values of the vector are radiuses of the
         *                ellipsoid. x and z should be equal. y should be
         *                bigger - the ellipsoid will look like a human body.
         */
        void setEllipsoid(const AVector &eRadius) { this->colPackage.eRadius = eRadius; }

        /**
         * Sets the level gravity.
         * This method sets the level gravity.
         * @param gravity Vector representing the level gravity
         * @see getGravityPosition
         * @see getGravityDirection
         */
        void setGravity(const AVector &gravity) { gravityVector = gravity; }

        /**
         * Enables gravity.
         * This method enables the gravity in the level.
         */
        void enableGravity() { this->gravity = true; }

        /**
         * Disables gravity.
         * This method disables the gravity in the level.
         */
        void disableGravity() { this->gravity = false; }

        /**
         * Returns gravity state.
         * @return True if the gravity is enabled
         * @see enableGravity
         * @see disableGravity
         * @see setGravity
         */
        bool isGravityEnabled() { return this->gravity; }

        /**
         * Enables collision detection Sphere test.
         * This method enables collision detection Sphere test. Collision
         * detection is done against the triangles being inside the sphere.
         * @see disableSphere
         * @see setSpherePosition
         * @see setSphereRadius
         */
        void enableSphere() { this->sphere = true; }

        /**
         * Disables collision detection Sphere test.
         * @see enableSphere
         * @see setSpherePosition
         * @see setSphereRadius
         */
        void disableSphere() { this->sphere = false; }

        /**
         * Sets new position of the sphere for collision detection.
         * This method sets new position of the sphere for collision detection
         * Sphere test.
         * @param sPosition Position of the center of the sphere
         * @see enableSphere
         * @see disablesphere
         * @see setSphereRadius
         */
        void setSpherePosition(const AVector &sPosition) { this->spherePosition = sPosition; }

        /**
         * Sets radius of the sphere for collision detection.
         * This method sets radius of the sphere for collision detection.
         * @param sRadius Radius of the sphere for collision detection
         * @see enableSphere
         * @see disablesphere
         * @see setSphereRadius
         */
        void setSphereRadius(double sRadius) { this->sphereRadius = sRadius; }
};

/**
 * Abstract class for collision detection.
 * This is abstract class for all objects doing collision against the level.
 */
class CollisionEllipsoid
{
    protected:
        Level *level;
        AVector eRadius;

    public:
        /**
         * Destructor.
         */
        virtual ~CollisionEllipsoid() {}

        /**
         * Sets the shape.
         * This method sets the shape of the ellipsoid.
         * @param eRadius Vector representing the ellipsoid. x, y and z
         *                values of the vector are radiuses of the
         *                ellipsoid. x and z should be equal. y should be
         *                bigger - the ellipsoid will look like a human body.
         */
        void setEllipsoid(const AVector &eRadius) { this->eRadius = eRadius; }
        /**
         * Sets the level.
         * This method sets the level that the collision detection is done against.
         * @param level Level to be set
         * @throws ANullPointerException
         */
        void setLevel(Level *level)
        {
            if (!level)
            {
                throw ANullPointerException("void CollisionEllipsoid::setLevel(Level *level)");
            }
            this->level = level;
        }

        /**
         * Returns the level.
         * This method returns the level that the collision detection is done against.
         * @return Level for collision detection
         */
        Level *getLevel() { return level; }

        /**
         * Reaction to the gravity of the level.
         */
        virtual void checkLevelGravity() = 0;
};

} // namespace astral3d

#endif    // #ifndef AABSTRACT_H
