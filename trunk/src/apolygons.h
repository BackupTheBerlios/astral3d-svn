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
 * @file apolygons.h ATriangle and AQuad structures.
 */
#ifndef APOLYGONS_H
#define APOLYGONS_H

#include <sstream>
#include <GL/gl.h>
#include "avector.h"
#include "aerror.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

/**
 * Structure representing a trinagle.
 * This structure represents a triangle.
 */
struct ATriangle
{
    AVector a, b, c;            // vertices
    double texCoordA[2];        // vertex A texture coordinates
    double texCoordB[2];        // vertex B texture coordinates
    double texCoordC[2];        // vertex C texture coordinates
    AVector normal;             // normal
    GLuint textureID;           // texture ID

    bool valid;                 // is trianle valid?

    /**
     * Computes the normal.
     * This method computes the normal of the tringle from its vertices.
     */
    void computeNormal();
};

/**
 * Structure representing a quad.
 * This structure represents a quad.
 */
struct AQuad
{
    AVector a, b, c, d;         // vertices
    double texCoordA[2];        // vertex A texture coordinates
    double texCoordB[2];        // vertex B texture coordinates
    double texCoordC[2];        // vertex C texture coordinates
    double texCoordD[2];        // vertex D texture coordinates
    AVector normal;             // normal
    GLuint textureID;           // texture ID

    bool valid;                 // is quad valid?

    /**
     * Computes the normal.
     * This method computes the normal of the quad from its vertices.
     */
    void computeNormal();
};

} // namespace astral3d

#endif // #ifndef APOLYGONS_H
