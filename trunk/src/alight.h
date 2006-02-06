 /*****************************************************************************
 * Astral3D is a 3D engine based on OpenGL and SDL.                           *
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
 * @file alight.h File defining ALight class.
 */
#ifndef ALIGHT_H
#define ALIGHT_H

#include <GL/gl.h>
#include <string>
#include <sstream>

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

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

    public:

        /**
         * Constructor.
         * Constructor.
         */
        ALight();
};

} // namespace astral3d

#endif    // #ifndef ALIGHT_H
