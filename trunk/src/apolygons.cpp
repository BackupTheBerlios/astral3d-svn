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

#include "apolygons.h"

namespace astral3d {

//-----------------------------------------------------------------------------
// pocita normalu
//-----------------------------------------------------------------------------

void ATriangle::computeNormal()
{
      AVector s1, s2;

      s1 = a - b;
      s2 = a - c;

      this->normal = s1 % s2;

      this->normal.normalize();
}

//-----------------------------------------------------------------------------
// pocita normalu
//-----------------------------------------------------------------------------

void AQuad::computeNormal()
{
      AVector s1, s2;

      s1 = a - b;
      s2 = a - c;

      this->normal = s1 % s2;

      this->normal.normalize();
}

} // namespace astral3d
