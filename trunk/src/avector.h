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
 * @file avector.h AVector class.
 */
#ifndef AVECTOR_H
#define AVECTOR_H

#ifdef WIN32
    #include <windows.h>
#endif

#include <iostream>
#include <sstream>
#include <cmath>

#include "aerror.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

//-----------------------------------------------------------------------------
// AVector class
//-----------------------------------------------------------------------------

/**
 * 3D space vector.
 * This class represents a vector in 3D space.
 */
class AVector
{
  friend class ACamera;

  public:
    double x, y, z;

    /**
     * Constructor.
     * Constructor.
     * @param x x-position
     * @param y y-position
     * @param z z-position
     */
    AVector(double x=0, double y=0, double z=0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /**
     * Sum.
     */
    AVector  operator+  (const AVector) const;
    /**
     * Difference.
     */
    AVector  operator-  (const AVector) const;
    /**
     * Unary minus.
     */
    AVector  operator-  (void) const;
    /**
     * Product.
     */
    AVector  operator*  (double) const;
    /**
     * Dot roduct.
     */
    double   operator*  (const AVector) const;
    /**
     * Cross roduct.
     */
    AVector  operator%  (const AVector) const;
    /**
     * Equivalence.
     */
    bool     operator== (const AVector) const;
    /**
     * Non equivalence.
     */
    bool     operator!= (const AVector) const;
    /**
     * Selection.
     */
    double   operator[] (int) const;

    /**
     * Normalizes the vector.
     * This method normalizes the vector.
     */
    void     normalize();
    /**
     * Applies the OpenGL matrix.
     * This method applies the OpenGL matrix to vector.
     * @param mat OpenGL matrix to apply
     */
    void     applyMatrix(double mat[16]);
    /**
     * Returns squared length.
     * This method returns squared length of the vector (for faster
     * calculations).
     * @return Squared length of the vector
     * @see getLength
     */
    double   squaredLength() { return (*this) * (*this); }
    /**
     * Sets the vector length.
     * This method sets the vector length.
     * @param length New vector length
     */
    void     setLength(double length);
    /**
     * Returns vector length.
     * This method returns vector length.
     * @return Vector length
     * @see squaredLength
     */
    double   getLength() { return sqrt((*this) * (*this)); }

    /**
     * Increments the vector.
     * @param a Vector to add
     */
    void     operator+= (const AVector a)   { (*this) = (*this) + a; }
    /**
     * Decrements the vector.
     * @param a Vector to subtract
     */
    void     operator-= (const AVector a)   { (*this) = (*this) - a; }
    /**
     * Cross product with the vector.
     * @param a Vector to multiply
     */
    void     operator%= (const AVector a)   { (*this) = (*this) % a; }
    /**
     * Product width the scalar.
     * @param m Scalar to multiply with
     */
    void     operator*= (double m)   { this->x*=m; this->y*=m; this->z*=m; }

    friend double mProduct(AVector, AVector, AVector);
    friend AVector operator*(double, const AVector);

    friend std::istream& operator>>(std::istream&, AVector&);
    friend std::ostream& operator<<(std::ostream&, const AVector&);

    friend  double abs(AVector);

    friend void setLength(AVector *v, double length);
};

} // namespace astral3d

#endif    // #ifndef AVECTOR_H
