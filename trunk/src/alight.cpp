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

#include "alight.h"

using namespace std;
namespace astral3d {

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
ALight::ALight()
{
    ambient[0] = 0.0f; ambient[1] = 0.0f; ambient[2] = 0.0f; ambient[3] = 1.0f;
    diffuse[0] = 1.0f; diffuse[1] = 1.0f; diffuse[2] = 1.0f; diffuse[3] = 1.0f;

    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;
    position[3] = 1.0f;
}

} // namespace astral3d
