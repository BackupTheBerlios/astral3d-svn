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

#include "avector.h"

using namespace std;
namespace astral3d {

//-----------------------------------------------------------------------------
// nastaveni delky vektoru
//-----------------------------------------------------------------------------

void setLength(AVector *v, double length)
{
    double len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
    v->x *= length/len;
    v->y *= length/len;
    v->z *= length/len;
}

//-----------------------------------------------------------------------------
// nastaveni delky vektoru
//-----------------------------------------------------------------------------

void AVector::setLength(double length)
{
    double len = getLength();
    x *= length/len;
    y *= length/len;
    z *= length/len;
}

//-----------------------------------------------------------------------------
// soucet vektoru
//-----------------------------------------------------------------------------

AVector AVector::operator+(const AVector b) const
{
    AVector result(*this);

    result.x+=b.x;
    result.y+=b.y;
    result.z+=b.z;

    return result;
}

//-----------------------------------------------------------------------------
// rozdil vektoru
//-----------------------------------------------------------------------------

AVector AVector::operator-(const AVector b) const
{
    AVector result(*this);

    result.x-=b.x;
    result.y-=b.y;
    result.z-=b.z;

    return result;
}

//-----------------------------------------------------------------------------
// opacny vektor
//-----------------------------------------------------------------------------

AVector AVector::operator-(void) const
{
    AVector result;

    result.x=(this->x!=0)?-this->x:0;
    result.y=(this->y!=0)?-this->y:0;
    result.z=(this->z!=0)?-this->z:0;

    return result;
}

//-----------------------------------------------------------------------------
// vektor * skalar
//-----------------------------------------------------------------------------

AVector AVector::operator*(double mult) const
{
    return mult*(*this);
}

//-----------------------------------------------------------------------------
// skalarni soucin
//-----------------------------------------------------------------------------

double AVector::operator*(const AVector b) const
{
    return this->x*b.x+this->y*b.y+this->z*b.z;
}

//-----------------------------------------------------------------------------
// vektorovy soucin
//-----------------------------------------------------------------------------

AVector AVector::operator%(const AVector b) const
{
    AVector result;

    result.x=this->y*b.z-this->z*b.y;
    result.y=this->z*b.x-this->x*b.z;
    result.z=this->x*b.y-this->y*b.x;

    return result;
}

//-----------------------------------------------------------------------------
// multi soucin
//-----------------------------------------------------------------------------

double mProduct(AVector a, AVector b, AVector c)
{
    return (a%b)*c;
}

//-----------------------------------------------------------------------------
// vstup
//-----------------------------------------------------------------------------

istream& operator>>(istream& in, AVector &a)
{
    in >> a.x >> a.y >> a.z;
    return in;
}

//-----------------------------------------------------------------------------
// vystup
//-----------------------------------------------------------------------------

ostream& operator<<(ostream& out, const AVector &a)
{
    out << a.x << " " << a.y << " " << a.z;
    return out;
}

//-----------------------------------------------------------------------------
// skalar * vektor
//-----------------------------------------------------------------------------

AVector operator*(double mult, const AVector a)
{
    AVector result;

    result.x=mult*a.x;
    result.y=mult*a.y;
    result.z=mult*a.z;

    return result;
}

//-----------------------------------------------------------------------------
// nozmalizace
//-----------------------------------------------------------------------------

void AVector::normalize()
{
    double size = abs(*this);

    if(size == 0) size = 1;

    this->x /= size;
    this->y /= size;
    this->z /= size;
}

//-----------------------------------------------------------------------------
// vraci x, y, nebo z
//-----------------------------------------------------------------------------

double AVector::operator[](int index) const
{
    switch(index)
    {
        case 0: return this->x; break;
        case 1: return this->y; break;
        case 2: return this->z; break;
    }

    return 0.0;
}

//-----------------------------------------------------------------------------
// velikost vektoru
//-----------------------------------------------------------------------------

double abs(AVector a)
{
    return sqrt(a*a);
}

//-----------------------------------------------------------------------------
// ekvivalence
//-----------------------------------------------------------------------------

bool AVector::operator==(const AVector b) const
{
    if((this->x==b.x) && (this->y==b.y) && (this->z==b.z)) return true;
    return false;
}

//-----------------------------------------------------------------------
// ! ekvivalence
//-----------------------------------------------------------------------

bool AVector::operator!=(const AVector b) const
{
    if(*this == b) return false;
    return true;
}

//-----------------------------------------------------------------------------
// upravi vektor podle OpenGL matice
//-----------------------------------------------------------------------------

void AVector::applyMatrix(double mat[16])
{
    double px, py, pz;

    px = this->x;
    py = this->y;
    pz = this->z;

    this->x = (px * mat[0]) + (py * mat[4]) + (pz * mat[8])  + (mat[12]);
    this->y = (px * mat[1]) + (py * mat[5]) + (pz * mat[9])  + (mat[13]);
    this->z = (px * mat[2]) + (py * mat[6]) + (pz * mat[10]) + (mat[14]);
}

} // namespace astral3d
