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

/*****************************************************************************
    N O T E:

    Written using the "Improved Collision detection and Response" article
    by Kasper Fauerby

    THX :)
******************************************************************************/

/**
 * @file acollision.h Collision detection functions.
 */
#ifndef ACOLLISION_H
#define ACOLLISION_H

#include <cmath>
#include <iostream>
#include <sstream>

#include "avector.h"
#include "aerror.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

#ifndef PI
    #define PI 3.1415926535897932
#endif

//-----------------------------------------------------------------------------
//  class for collisions
//-----------------------------------------------------------------------------

/**
 * Structure containing information about the collision.
 */
struct ACollisionPacket
{
        // radius of the ellipsoid
        AVector eRadius;

        // info about the requested move in the R3 vector space
        AVector r3Velocity;
        AVector r3Position;

        // info about the requested move in the ellipsoid vector space
        AVector velocity;
        AVector normalizedVelocity;
        AVector basePoint;

        // info about the collision
        bool foundCollision;
        double nearestDistance;
        AVector intersectionPoint;

        /**
         * Prints the info.
         * Prints information to stdout
         */
        void print();
};

//-----------------------------------------------------------------------------
//  plane class
//-----------------------------------------------------------------------------

/**
 * Structure describing a plane in the 3D space.
 */
struct APlane
{
        double equation[4];
        AVector origin;
        AVector normal;

    /**
     * Constructor.
     * Constructor from the vertex and the normal of the plane.
     * @param origin Vertex of the plane
     * @param normal Normal of the plane
     */
    APlane(const AVector& origin, const AVector& normal);

    /**
     * Constructor.
     * Constructor from the triangle.
     * @param p1 Vertex A of the triangle
     * @param p2 Vertex B of the triangle
     * @param p3 Vertex C of the triangle
     */
    APlane(const AVector& p1, const AVector& p2, const AVector& p3);

    /**
     * Is front facing to the point.
     * @param direction Point to be tested against
     * @return True if the plane has its front face to the given point
     */
    bool isFrontFacingTo(const AVector& direction) const;

    /**
     * Distance to the given point.
     * @param point Point to be testes against
     * @return Signed distance to the given point
     */
    double signedDistanceTo(const AVector& point) const;
};

/**
 * Checkes if the point is inside the triangle.
 */
bool checkPointInTriangle(const AVector& point, const AVector& pa,const AVector& pb, const AVector& pc);
/**
 * Solves the equation.
 */
bool getLowestRoot(double a, double b, double c, double maxR, double* root);
/**
 * Tests the collision detection against the triangle.
 */
void checkTriangle(ACollisionPacket* colPackage, const AVector& p1,const AVector& p2,const AVector& p3, const AVector& normal);
/**
 * Calculates new position after the sliding.
 */
bool slide(ACollisionPacket* colPackage, double unitsPerMeter, AVector *newBasePoint, AVector *newVelocityVector);

} // namespace astral3d

#endif // #ifndef ACOLLISION_H
