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

/*****************************************************************************
    N O T E:

    Written using the "Improved Collision detection and Response" article
    by Kasper Fauerby

    THX :)
******************************************************************************/

/**
 * @file acollision.h File defining classes and functions for the collision detection.
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
 * Class containing information about the collision.
 * This class contains information about the collision in 3D space.
 */
class ACollisionPacket
{
    public:
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
         * Prints the information to the standart output
         */
        void print();
};

//-----------------------------------------------------------------------------
//  plane class
//-----------------------------------------------------------------------------

/**
 * Class describing a plane in the 3D space.
 * This class contains an information to describe a plane in the 3D space.
 */
class APlane
{
    public:
        double equation[4];
        AVector origin;
        AVector normal;

    /**
     * Constructor.
     * Constructor from the vertex and normal of the plane.
     * @param origin Vertex of the plane
     * @param normal Normal of the plane
     */
    APlane(const AVector& origin, const AVector& normal);

    /**
     * Constructor.
     * Constructor from the triangle.
     * @param p1 vertex A of the triangle
     * @param p2 vertex B of the triangle
     * @param p3 vertex C of the triangle
     */
    APlane(const AVector& p1, const AVector& p2, const AVector& p3);

    bool isFrontFacingTo(const AVector& direction) const;

    double signedDistanceTo(const AVector& point) const;
};


bool checkPointInTriangle(const AVector& point, const AVector& pa,const AVector& pb, const AVector& pc);
bool getLowestRoot(double a, double b, double c, double maxR, double* root);
void checkTriangle(ACollisionPacket* colPackage, const AVector& p1,const AVector& p2,const AVector& p3, const AVector& normal);
bool slide(ACollisionPacket* colPackage, double unitsPerMeter, AVector *newBasePoint, AVector *newVelocityVector);

} // namespace astral3d

#endif // #ifndef ACOLLISION_H
