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

/******************************************************************************
 N O T E:

 Written thanks the great project APRON TUTORIALS at http://www.morrowland.com

******************************************************************************/


/**
 * @file a3ds.h 3DS file loaders.
 */
#ifndef A3DS_H
#define A3DS_H

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#include <fstream>
#include <cmath>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef WIN32
    #include <SDL.h>
#else
    #include "SDL.h"
#endif

#include "aerror.h"


/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

#ifndef PI
    #define PI 3.1415926535897932
#endif

#define BEHIND      0
#define INTERSECTS  1
#define FRONT       2

// Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

// Main Chunks
#define OBJECTINFO    0x3D3D
#define VERSION       0x0002
#define EDITKEYFRAME  0xB000

// Sub defines of OBJECTINFO
#define MATERIAL      0xAFFF
#define OBJECT        0x4000

// Sub defines of MATERIAL
#define MATNAME       0xA000
#define MATDIFFUSE    0xA020
#define MATMAP        0xA200
#define MATMAPFILE    0xA300

#define OBJECT_MESH   0x4100

// Sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110
#define OBJECT_FACES        0x4120
#define OBJECT_MATERIAL     0x4130
#define OBJECT_UV           0x4140

//-----------------------------------------------------------------------------
// 3D vector
//-----------------------------------------------------------------------------
struct AVector3
{
    float x, y, z;

    AVector3() {}
    AVector3 (float new_x, float new_y, float new_z)
    {x = new_x; y = new_y; z = new_z;}

    AVector3 operator+(AVector3 vVector) {return AVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
    AVector3 operator-(AVector3 vVector) {return AVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
    AVector3 operator*(float number)     {return AVector3(x*number, y*number, z*number);}
    AVector3 operator/(float number)     {return AVector3(x/number, y/number, z/number);}
};

//-----------------------------------------------------------------------------
// 2D vector
//-----------------------------------------------------------------------------
struct AVector2
{
    float x, y;
};

// vector functions
float     absolute(float num);
AVector3  Cross(AVector3 vVector1, AVector3 vVector2);
float     Dot(AVector3 vVector1, AVector3 vVector2);
AVector3  Normalize(AVector3 vNormal);
AVector3  Normal(AVector3 vTriangle[]);
float     Magnitude(AVector3 vNormal);

//-----------------------------------------------------------------------------
// Face
//-----------------------------------------------------------------------------
struct AFace
{
    int vertIndex[3];
    int coordIndex[3];
};

//-----------------------------------------------------------------------------
// Material Info
//-----------------------------------------------------------------------------
/**
 * Material info of the object in A3DModel class.
 */
struct AMaterialInfo
{
    char  strName[255];
    char  strFile[255];
    int   color[3];
    int   texureId;
    float uTile;
    float vTile;
    float uOffset;
    float vOffset;
};

//-----------------------------------------------------------------------------
// 3D Object
//-----------------------------------------------------------------------------
/**
 * 3D object building 3D model represented by A3DModel class.
 */
struct A3DObject
{
    int  numOfVerts;
    int  numOfFaces;
    int  numTexVertex;
    int  materialID;
    bool bHasTexture;
    char strName[255];
    GLuint    *pIndices;
    AVector3  *pVerts;
    AVector3  *pNormals;
    AVector2  *pTexVerts;
    AFace *pFaces;
};

//-----------------------------------------------------------------------------
// 3D Model
//-----------------------------------------------------------------------------
/**
 * Structure containing model data.
 * This structure contains data of the 3D model. Used by Model3D class.
 */
struct A3DModel
{
    int numOfObjects;
    int numOfMaterials;
    std::vector<AMaterialInfo> pMaterials;
    std::vector<A3DObject> pObject;
};

//-----------------------------------------------------------------------------
// Indices
//-----------------------------------------------------------------------------
struct AIndices {
    unsigned short a, b, c, bVisible;
};

//-----------------------------------------------------------------------------
// Chunk
//-----------------------------------------------------------------------------
struct AChunk
{
    unsigned short int ID;
    unsigned int length;
    unsigned int bytesRead;
};

//-----------------------------------------------------------------------------
// 3DS Loader
//-----------------------------------------------------------------------------
class A3DSLoader
{
public:
    A3DSLoader();

    bool load3DSModel(A3DModel *pModel, char *strFileName);

private:
    int getString(char *);

    void readChunk(AChunk *);

    void processNexAChunk(A3DModel *pModel, AChunk *);

    void processNextObjecAChunk(A3DModel *pModel, A3DObject *pObject, AChunk *);

    void processNextMaterialChunk(A3DModel *pModel, AChunk *);

    void readColorChunk(AMaterialInfo *pMaterial, AChunk *pChunk);

    void readVertices(A3DObject *pObject, AChunk *);

    void readVertexIndices(A3DObject *pObject, AChunk *);

    void readUVCoordinates(A3DObject *pObject, AChunk *);

    void readObjectMaterial(A3DModel *pModel, A3DObject *pObject, AChunk *pPreviousChunk);

    void computeNormals(A3DModel *pModel);

    void cleanUp();

    FILE *m_FilePointer;

    AChunk *m_CurrenAChunk;
    AChunk *m_TempChunk;
};

} // namespace astral3d

#endif    // #ifndef A3DS_H
