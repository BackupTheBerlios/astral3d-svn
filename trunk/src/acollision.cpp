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
    POZNAMKA:

    Zpracovano podle clanku "Improved Collision detection and Response"
    autor: Kasper Fauerby

    THX :)
******************************************************************************/

#include "acollision.h"

using namespace std;
namespace astral3d {


//-----------------------------------------------------------------------------
//  tiskne informace o kolizni strukture
//-----------------------------------------------------------------------------

void ACollisionPacket::print()
{
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "eRadius = " << eRadius << std::endl;
    std::cout << "r3Velocity = " << r3Velocity << std::endl;
    std::cout << "r3Position = " << r3Position << std::endl;
    std::cout << "velocity = " << velocity << std::endl;
    std::cout << "normalizedVelocity = " << normalizedVelocity << std::endl;
    std::cout << "basePoint = " << basePoint << std::endl;
    std::cout << "foundCollision = " << foundCollision << std::endl;
    std::cout << "nearestDistance = " << nearestDistance << std::endl;
    std::cout << "intersectionPoint = " << intersectionPoint << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
}

//-----------------------------------------------------------------------------
//
//  APlane
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// konstruktor z bodu plochy a jeji normaly
//-----------------------------------------------------------------------------

APlane::APlane(const AVector& origin, const AVector& normal)
{
    this->normal = normal;
    this->origin = origin;

    equation[0] = normal.x;
    equation[1] = normal.y;
    equation[2] = normal.z;
    equation[3] = -(normal.x*origin.x+normal.y*origin.y + normal.z*origin.z);
}

//-----------------------------------------------------------------------------
// konstruktor z trojuhelniku
//-----------------------------------------------------------------------------

APlane::APlane(const AVector& p1,const AVector& p2, const AVector& p3)
{
    normal = (p2-p1) % (p3-p1);
    normal.normalize();
    origin = p1;
    equation[0] = normal.x;
    equation[1] = normal.y;
    equation[2] = normal.z;
    equation[3] = -(normal.x*origin.x+normal.y*origin.y +normal.z*origin.z);
}

//-----------------------------------------------------------------------------
// test je-li bod pred plochou
//-----------------------------------------------------------------------------

bool APlane::isFrontFacingTo(const AVector& direction) const
{
    double dot = normal * direction;
    return (dot <= 0);
}

//-----------------------------------------------------------------------------
// pocita vdalenost k bodu
//-----------------------------------------------------------------------------

double APlane::signedDistanceTo(const AVector& point) const
{
    return (point * normal) + equation[3];
}

//-----------------------------------------------------------------------------
//
//  pomocne funkce
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// test zda-li je bod uvnitr trojuhelniku
//-----------------------------------------------------------------------------

bool checkPointInTriangle(const AVector& point, const AVector& pa, const AVector& pb, const AVector& pc)
{
    double total_angles = 0.0f;

    AVector v1 = point-pa;
    AVector v2 = point-pb;
    AVector v3 = point-pc;

    v1.normalize();
    v2.normalize();
    v3.normalize();

    total_angles += acos(v1 * v2);
    total_angles += acos(v2 * v3);
    total_angles += acos(v3 * v1);

    if (fabs(total_angles-2*PI) <= 0.005)
        return true;

    return false;
}

//-----------------------------------------------------------------------------
// resi kvadratickou rovnici
//-----------------------------------------------------------------------------

bool getLowestRoot(double a, double b, double c, double maxR, double* root)
{
    double determinant = b*b - 4.0f*a*c;
    // pokud je determinant zaporny, reseni neexistuje
    if (determinant < 0.0f)
        return false;
    // spocita dva koreny rovnice
    double sqrtD = sqrt(determinant);
    double r1 = (-b - sqrtD) / (2*a);
    double r2 = (-b + sqrtD) / (2*a);
    // setridime je tak, aby platilo r1 < r2
    if (r1 > r2)
    {
        double temp = r2;
        r2 = r1;
        r1 = temp;
    }
    // vrati nejmensi koren reseni
    if (r1 > 0 && r1 < maxR)
    {
        *root = r1;
        return true;
    }
    // pokud nevyhovuje r1, vracime r2
    if (r2 > 0 && r2 < maxR)
    {
        *root = r2;
        return true;
    }
    // neni zadne platne reseni
    return false;
}

//-----------------------------------------------------------------------------
// provadi detekci kolize s trojuhelnikem
//-----------------------------------------------------------------------------

void checkTriangle(ACollisionPacket* colPackage, const AVector& p1,const AVector& p2,const AVector& p3, const AVector& normal)
{
    // vytvorime plochu z trojuhelniku, ktery na kolizi testujeme
    APlane trianglePlane(p1, normal);

    // test budeme provadet pouze s plochami, ktere jsou k nam natoceny
    // predni stranou - to nam v realu hodne pomuze a zrychli nam cely proces
    if (trianglePlane.isFrontFacingTo(colPackage->normalizedVelocity))
    {
        // interval, ve kterem kolizni elipsoid je v kolizi s plochou
        // (vezmeme si kouli pohybujici se proti plose, v okamziku t0 dojde
        // k prvnimu kontaktu, pak se pohybuje skrz a v okamziku t1 nastane
        // posledni kontakt) t0 a t1 jsou v rozmezi 0 az 1. (0=0% delky
        // vektoru pohybu koule skrz plochu resp. 1=100% delky tohoto vektoru)
        double t0, t1;
        bool embeddedInPlane = false;

        // vzdalenost k plose
        double signedDistToTrianglePlane = trianglePlane.signedDistanceTo(colPackage->basePoint);

        double normalDotVelocity = trianglePlane.normal*colPackage->velocity;

        // pokud se kolizni koule pohybuje paralelne k plose
        if (normalDotVelocity == 0.0f)
        {
            if (fabs(signedDistToTrianglePlane) >= 1.0f)
            {
                // kolize neuze nastat
                return;
            }
            else
            {
                // protina plochu na celem intervalu 0..1 (viz vyse)
                embeddedInPlane = true;
                t0 = 0.0;
                t1 = 1.0;
            }
        }
        else
        {
            // musime spocitat interval t0 .. t1 kdy je kolizni koule
            // v kontaktu s plochou
            t0=(-1.0-signedDistToTrianglePlane)/normalDotVelocity;
            t1=( 1.0-signedDistToTrianglePlane)/normalDotVelocity;

            // chceme mit t0 < t1, tzn. v pripade potreby prohodit :)
            if (t0 > t1)
            {
                double temp = t1;
                t1 = t0;
                t0 = temp;
            }

            // aspon jeden musi byt v korektnim rozmezi 0 .. 1
            if (t0 > 1.0f || t1 < 0.0f)
            {
                // obe hodnoty jsou mimo pripustny interval
                // => kolize nemuze nastat
                return;
            }
                // nastavime do spravneho rozmezi pokud jsou mimo
                if (t0 < 0.0) t0 = 0.0;
                if (t1 < 0.0) t1 = 0.0;
                if (t0 > 1.0) t0 = 1.0;
                if (t1 > 1.0) t1 = 1.0;
        }
        // Nyni mame interval t0 .. t1 nastaveny, pokud se tedy
        // vyskytla kolize musi byt v tomto intervalu, tzn. kolize sice
        // nastat muze, ale s plochou tvorenou trojuhelnikem, tato plocha je
        // vsak nekonecna a trojuhelnik je jeji soucasti

        // tento bod je bod dotyku kulizni koule a plochy
        AVector collisionPoint;
        bool foundCollison = false;
        double t = 1.0;

        // Nejdrive testujeme jednodussi moznost. Protoze pokud kolize
        // nastane uvnitr trojuhelniku, bude to prave v intervalu t0, tedy
        // v okamziku, kdy se kolizni koule poprve dotkne plochy
        // To muze nastat pouze tehdy pokud kolizni koule plochu neprotina
        // vsude => testujeme vyse spocitanou embeddedInPlane promennou

        if (!embeddedInPlane)
        {
            // bod dotyku
            AVector planeIntersectionPoint = (colPackage->basePoint-trianglePlane.normal) + t0*colPackage->velocity;

            // je bod dotyku uvnitr trojuhelniku?
            if(checkPointInTriangle(planeIntersectionPoint, p1,p2,p3))
            {
                // je :)
                foundCollison = true;
                t = t0;
                collisionPoint = planeIntersectionPoint;
            }
        }

        // Tak, tohle je ta tezsi moznost, kolize s kolizni kouli nastava
        // bud ve vrcholu trojuhelniku nebo na jeho hrane!
        // Pokud nastane kolize uvnitr trojuhelniku, nastane VZDY drive nez
        // kolize s vrcholem nebo hranou

        if (foundCollison == false)
        {
            AVector velocity = colPackage->velocity;
            AVector base = colPackage->basePoint;
            double velocitysquaredLength = velocity.squaredLength();
            double a,b,c;
            double newT;

            // pro kazdy vrchol a hranu resime kvadratickou rovnici
            // a*t^2 + b*t + c = 0

            // kontrola vuci vrcholum

            a = velocitysquaredLength;
            // vrchol 1
            b = 2.0*(velocity*(base-p1));
            c = (p1-base).squaredLength() - 1.0;
            if (getLowestRoot(a,b,c, t, &newT))
            {
                t = newT;
                foundCollison = true;
                collisionPoint = p1;
            }
            // vrchol 2
            b = 2.0*(velocity*(base-p2));
            c = (p2-base).squaredLength() - 1.0;
            if (getLowestRoot(a,b,c, t, &newT))
            {
                t = newT;
                foundCollison = true;
                collisionPoint = p2;
            }
            // vrchol 3
            b = 2.0*(velocity*(base-p3));
            c = (p3-base).squaredLength() - 1.0;
            if (getLowestRoot(a,b,c, t, &newT))
            {
                t = newT;
                foundCollison = true;
                collisionPoint = p3;
            }

            // kontrola vuci hranam
            // hrana vrchol 1 -> vrchol 2:
            AVector edge = p2-p1;
            AVector baseToVertex = p1 - base;
            double edgesquaredLength = edge.squaredLength();
            double edgeDotVelocity = edge*(velocity);
            double edgeDotBaseToVertex = edge*(baseToVertex);

            // vypocet parametru pro rovnici
            a = edgesquaredLength*-velocitysquaredLength + edgeDotVelocity*edgeDotVelocity;
            b = edgesquaredLength*(2*velocity*(baseToVertex)) - 2.0*edgeDotVelocity*edgeDotBaseToVertex;
            c = edgesquaredLength*(1-baseToVertex.squaredLength()) + edgeDotBaseToVertex*edgeDotBaseToVertex;

            // test kolize s "nekonecnou" hranou
            if (getLowestRoot(a,b,c, t, &newT))
            {
                // pokud kolize s nekonecnou hranou nastala, musime tedy zjistit,
                // jestli nastala na nasi hrane mezi dvema vrcholy
                double f=(edgeDotVelocity*newT-edgeDotBaseToVertex) / edgesquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    // ano, kolize je na hrane
                    t = newT;
                    foundCollison = true;
                    collisionPoint = p1 + f*edge;
                }
            }
            // hrana vrchol 2 -> vrchol 3:
            edge = p3-p2;
            baseToVertex = p2 - base;
            edgesquaredLength = edge.squaredLength();
            edgeDotVelocity = edge*(velocity);
            edgeDotBaseToVertex = edge*(baseToVertex);

            a = edgesquaredLength*-velocitysquaredLength + edgeDotVelocity*edgeDotVelocity;
            b = edgesquaredLength*(2*velocity*(baseToVertex)) - 2.0*edgeDotVelocity*edgeDotBaseToVertex;
            c = edgesquaredLength*(1-baseToVertex.squaredLength()) + edgeDotBaseToVertex*edgeDotBaseToVertex;

            if (getLowestRoot(a,b,c, t, &newT))
            {
                double f=(edgeDotVelocity*newT-edgeDotBaseToVertex) / edgesquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    t = newT;
                    foundCollison = true;
                    collisionPoint = p2 + f*edge;
                }
            }
            // hrana vrchol 3 -> vrchol 1:
            edge = p1-p3;
            baseToVertex = p3 - base;
            edgesquaredLength = edge.squaredLength();
            edgeDotVelocity = edge*(velocity);
            edgeDotBaseToVertex = edge*(baseToVertex);

            a = edgesquaredLength*-velocitysquaredLength + edgeDotVelocity*edgeDotVelocity;
            b = edgesquaredLength*(2*velocity*(baseToVertex)) - 2.0*edgeDotVelocity*edgeDotBaseToVertex;
            c = edgesquaredLength*(1-baseToVertex.squaredLength()) + edgeDotBaseToVertex*edgeDotBaseToVertex;

            if (getLowestRoot(a,b,c, t, &newT))
            {
                double f=(edgeDotVelocity*newT-edgeDotBaseToVertex) / edgesquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    t = newT;
                    foundCollison = true;
                    collisionPoint = p3 + f*edge;
                }
            }
        }

        // nakonec nastavime vysledek do kolizni struktury
        if (foundCollison == true)
        {
            // vzdalenost ke kolizi, t je okamzik kolize
            double distToCollision = t*colPackage->velocity.getLength();

            if (colPackage->foundCollision == false || distToCollision < colPackage->nearestDistance)
            {
                // potrebne informace pro vypocet slidingu
                colPackage->nearestDistance = distToCollision;
                colPackage->intersectionPoint=collisionPoint;
                colPackage->foundCollision = true;
            }
        }
    } // --> if (trianglePlane.isFrontFacingTo(colPackage->normalizedVelocity))
}

//-----------------------------------------------------------------------------
// provadi vypocet noveho vychoziho bodu a noveho smeru pohybu
// vraci true pokud je treba volat celou kolizni funkci rekurzivne znovu
//-----------------------------------------------------------------------------

bool slide(ACollisionPacket* colPackage, double unitsPerMeter, AVector *newBasePoint, AVector *newVelocityVector)
{
    // nastaveni mezi
    double unitScale = unitsPerMeter / 100.0f;
    double veryCloseDistance = 0.005f * unitScale;

    AVector pos = colPackage->basePoint;
    AVector vel = colPackage->velocity;

    // pokud nenastala kolize, neni co resit :)
    if (colPackage->foundCollision == false)
    {
        // novy bod je jednoduse vychozi s prictenim vektoru pohybu
        *newBasePoint = pos + vel;
        return false;
    }

    // kolize nastala

    // spocitame pozadovany cilovy bod
    AVector destinationPoint = pos + vel;
    *newBasePoint = pos;

    // pokud jsme vzdaleni od kolizni plochy, upravime novy vychozi bod tak,
    // aby se kolizni elipsoid "dotykal" plochy (s prihlednutim na mesi rozdily,
    // ktere nam udava veryCloseDistance => musime pak take upravit kolizni
    // bod, abysme pote spravne vypocitali plochu pro sliding)
    if (colPackage->nearestDistance>=veryCloseDistance)
    {
        AVector V = vel;
        V.setLength(colPackage->nearestDistance-veryCloseDistance);
        *newBasePoint = colPackage->basePoint + V;
        // upravime take kolizni bod (aby plocha pro sliding nebyla
        // ovlivnena faktem, ze jsme novy vychozi bod nenastavili presne
        // na dotyk, ale mirne dal podle veryCloseDistance)
        V.normalize();
        colPackage->intersectionPoint -= veryCloseDistance * V;
    }

    // vypocet plochy pro sliding
    AVector slidePlaneOrigin = colPackage->intersectionPoint;
    AVector slidePlaneNormal = *newBasePoint-colPackage->intersectionPoint;
    slidePlaneNormal.normalize();
    APlane slidingPlane(slidePlaneOrigin,slidePlaneNormal);

    AVector newDestinationPoint = destinationPoint - slidingPlane.signedDistanceTo(destinationPoint) * slidePlaneNormal;

    // vypocet noveho vektoru pohybu
    *newVelocityVector = newDestinationPoint - colPackage->intersectionPoint;

    // opet mame moznost vratit false, jde o to, ze pokud spocitame novy vychozi
    // bod a novy vektor pohybu, musime opet provadet test kolize, pro tyto nove
    // hodnoty, abychom se napr. nezasekli v rohu, kdy provedeme sice test a
    // nasledny sliding po jedne stene, ale zasekneme se ve druhe
    // pokud tedy velikost noveho vektoru pohybu bude hodne mala, uz nebereme
    // v potaz, ze k nejakemu pohybu vubec dojde a vratime pouze vyslednou pozici
    // (pokud tedy vratime false uz k zadnemu pohybu take dojit nesmi :-)

    if (newVelocityVector->getLength() < veryCloseDistance)
        return false;

    return true;
}

} // namespace astral3d
