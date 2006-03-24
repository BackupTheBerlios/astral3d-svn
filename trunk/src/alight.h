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
 * @file alight.h ALight class.
 */
#ifndef ALIGHT_H
#define ALIGHT_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <sstream>

#include "avector.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/**
 * Class for displaying lights.
 * This class provides basic functions for displaying lights in the scene.
 */
class ALight
{

    private:

        GLfloat ambient[4];
        GLfloat diffuse[4];
        GLfloat position[4];

        GLenum light;   // lights ID

    public:

        /**
         * Constructor.
         */
        ALight(GLenum light);

        /**
         * Sets the ambient element.
         * @param r Red value (0.0 to 1.0)
         * @param g Green value (0.0 to 1.0)
         * @param b Blue value (0.0 to 1.0)
         * @param a Alpha value (0.0 to 1.0)
         */
        inline void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

        /**
         * Sets the diffuse element.
         * @param r Red value (0.0 to 1.0)
         * @param g Green value (0.0 to 1.0)
         * @param b Blue value (0.0 to 1.0)
         * @param a Alpha value (0.0 to 1.0)
         */
        inline void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

        /**
         * Sets the light position.
         * @param x x-position
         * @param y y-position
         * @param z z-position
         * @param r r-position
         */
        inline void setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat r = 1.0f);

        /**
         * Sets the light position.
         * @param position New light position
         */
        inline void setPosition(AVector position);

        /**
         * Updates the light position
         * @param position Vector to add to current position
         */
        inline void update(AVector vector);

        /**
         * Renderes the light.
         * This method draws the sphere to to light position.
         */
        void render(double radius);

        /**
         * Disables light.
         */
        void disable() { glDisable(light); }

        /**
         * Enables light.
         */
        void enable() { glEnable(light); }

        /**
         * Sets the light to its position.
         */
        void set() { glLightfv(light, GL_POSITION, position); }
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ALight::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    ambient[0] = r;
    ambient[1] = g;
    ambient[2] = b;
    ambient[3] = a;

    glLightfv(light, GL_AMBIENT, ambient);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ALight::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
    diffuse[3] = a;

    glLightfv(light, GL_DIFFUSE, diffuse);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ALight::setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
    position[3] = r;

    glLightfv(light, GL_POSITION, position);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ALight::setPosition(AVector position)
{
    this->position[0] = position.x;
    this->position[1] = position.y;
    this->position[2] = position.z;

    glLightfv(light, GL_POSITION, this->position);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ALight::update(AVector vector)
{
    position[0] += vector.x;
    position[1] += vector.y;
    position[2] += vector.z;

    glLightfv(light, GL_POSITION, this->position);
}

} // namespace astral3d

#endif    // #ifndef ALIGHT_H
