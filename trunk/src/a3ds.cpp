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

/******************************************************************************
 N O T E:

 Written thanks the great project APRON TUTORIALS at http://www.morrowland.com

******************************************************************************/

#include "a3ds.h"

using namespace std;
namespace astral3d {

//-----------------------------------------------------------------------------
// vector functions
//-----------------------------------------------------------------------------

float Absolute(float num)
{
    if(num < 0) return (0 - num);
    return num;
}

AVector3 Cross(AVector3 vVector1, AVector3 vVector2)
{
    AVector3 vNormal;

    vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
    vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
    vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

    return vNormal;
}

float Dot(AVector3 vVector1, AVector3 vVector2)
{
    return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) );
}

AVector3 Normalize(AVector3 vNormal)
{
    float magnitude = Magnitude(vNormal);

    vNormal.x /= magnitude;
    vNormal.y /= magnitude;
    vNormal.z /= magnitude;

    return vNormal;
}

AVector3 Normal(AVector3 vTriangle[])
{
    AVector3 vVector1 = vTriangle[2] - vTriangle[0];
    AVector3 vVector2 = vTriangle[1] - vTriangle[0];
    AVector3 vNormal = Cross(vVector1, vVector2);
    vNormal = Normalize(vNormal);

    return vNormal;
}

float Magnitude(AVector3 vNormal)
{
    return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) );
}

//-----------------------------------------------------------------------------
// This constructor initializes the AChunk data
//-----------------------------------------------------------------------------

A3DSLoader::A3DSLoader()
{
    m_CurrenAChunk = new AChunk;
    m_TempChunk    = new AChunk;
}

//-----------------------------------------------------------------------------
// This is called by the client to open the .3ds file, read it, then clean up
//-----------------------------------------------------------------------------

bool A3DSLoader::load3DSModel(A3DModel *pModel, char *strFileName)
{

#ifdef DEBUG
    cout << "vstupuji do metody load3DSModel tridy A3DSLoader" << endl;
#endif

    pModel->numOfMaterials=0;
    pModel->numOfObjects=0;

#ifdef DEBUG
    cout << "oteviram soubor \"" << strFileName << "\"" << endl;
#endif

    m_FilePointer = NULL;

    m_FilePointer = fopen(strFileName, "rb");

    if(m_FilePointer == NULL)
    {
        stringstream foo;
        stringstream bar;
        foo << "A3DSLoader::load3DSModel("<<pModel<<", "<<strFileName<<")";
        bar << "fopen("<<strFileName<<", "<<"\"rb\")";
        setAstral3DError("Can't open file", foo.str(), bar.str());
        return false;
    }

#ifdef DEBUG
    cout << "soubor otevren" << endl;
    cout << "nacitam data z modelu" << endl;
#endif

    readChunk(m_CurrenAChunk);

    if (m_CurrenAChunk->ID != PRIMARY)
    {
        stringstream foo;
        stringstream bar;
        foo << "A3DSLoader::load3DSModel("<<pModel<<", \""<<strFileName<<"\")";
        bar << "A3DSLoader::readChunk("<<m_CurrenAChunk<<")";
        setAstral3DError("Unable to load PRIMARY chuck", foo.str(), bar.str());
        return false;
    }

#ifdef DEBUG
    cout << "data nactena" << endl;
    cout << "nacitam dalsi data z modelu" << endl;
#endif

    processNexAChunk(pModel, m_CurrenAChunk);

#ifdef DEBUG
    cout << "dalsi data nactena" << endl;
    cout << "pocitam normaly" << endl;
#endif

    computeNormals(pModel);

#ifdef DEBUG
    cout << "normaly spocitany" << endl;
    cout << "volam uklid" << endl;
#endif

    cleanUp();

#ifdef DEBUG
    cout << "pomocna pamet uvolnena" << endl;
#endif

    return true;
}

//-----------------------------------------------------------------------------
// This function cleans up our allocated memory and closes the file
//-----------------------------------------------------------------------------

void A3DSLoader::cleanUp()
{
#ifdef DEBUG
    cout << "uklid: zaviram soubor s modelem" << endl;
#endif

    if (m_FilePointer) {
        fclose(m_FilePointer);
    }

#ifdef DEBUG
    cout << "uklid: soubor uzavren" << endl;
#endif

    delete m_CurrenAChunk;
    delete m_TempChunk;
}


//-----------------------------------------------------------------------------
// This function reads the main sections of the .3DS file, then dives deeper with recursion
//-----------------------------------------------------------------------------

void A3DSLoader::processNexAChunk(A3DModel *pModel, AChunk *pPreviousChunk)
{
    A3DObject newObject = {0};
    AMaterialInfo newTexture = {0};
    unsigned short version = 0;
    int buffer[50000] = {0};

    m_CurrenAChunk = new AChunk;

    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
        readChunk(m_CurrenAChunk);

        switch (m_CurrenAChunk->ID)
        {
        case VERSION:

            m_CurrenAChunk->bytesRead += fread(&version, 1, m_CurrenAChunk->length - m_CurrenAChunk->bytesRead, m_FilePointer);

            if (version > 0x03);
                //MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
            break;

        case OBJECTINFO:
            readChunk(m_TempChunk);

            m_TempChunk->bytesRead += fread(&version, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);

            m_CurrenAChunk->bytesRead += m_TempChunk->bytesRead;

            processNexAChunk(pModel, m_CurrenAChunk);
            break;

        case MATERIAL:
            pModel->numOfMaterials++;

            pModel->pMaterials.push_back(newTexture);

            processNextMaterialChunk(pModel, m_CurrenAChunk);
            break;

        case OBJECT:
            pModel->numOfObjects++;

            pModel->pObject.push_back(newObject);

            memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(A3DObject));

            m_CurrenAChunk->bytesRead += getString(pModel->pObject[pModel->numOfObjects - 1].strName);

            processNextObjecAChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_CurrenAChunk);
            break;

        case EDITKEYFRAME:

            m_CurrenAChunk->bytesRead += fread(buffer, 1, m_CurrenAChunk->length - m_CurrenAChunk->bytesRead, m_FilePointer);
            break;

        default:

            m_CurrenAChunk->bytesRead += fread(buffer, 1, m_CurrenAChunk->length - m_CurrenAChunk->bytesRead, m_FilePointer);
            break;
        }

        pPreviousChunk->bytesRead += m_CurrenAChunk->bytesRead;
    }

    delete m_CurrenAChunk;
    m_CurrenAChunk = pPreviousChunk;
}


//-----------------------------------------------------------------------------
// This function handles all the information about the objects in the file
//-----------------------------------------------------------------------------

void A3DSLoader::processNextObjecAChunk(A3DModel *pModel, A3DObject *pObject, AChunk *pPreviousChunk)
{
    int buffer[50000] = {0};

    m_CurrenAChunk = new AChunk;

    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
        readChunk(m_CurrenAChunk);

        switch (m_CurrenAChunk->ID)
        {
        case OBJECT_MESH:
            processNextObjecAChunk(pModel, pObject, m_CurrenAChunk);
            break;

        case OBJECT_VERTICES:
            readVertices(pObject, m_CurrenAChunk);
            break;

        case OBJECT_FACES:
            readVertexIndices(pObject, m_CurrenAChunk);
            break;

        case OBJECT_MATERIAL:
            readObjectMaterial(pModel, pObject, m_CurrenAChunk);
            break;

        case OBJECT_UV:
            readUVCoordinates(pObject, m_CurrenAChunk);
            break;

        default:
            m_CurrenAChunk->bytesRead += fread(buffer, 1, m_CurrenAChunk->length - m_CurrenAChunk->bytesRead, m_FilePointer);
            break;
        }

        pPreviousChunk->bytesRead += m_CurrenAChunk->bytesRead;
    }

    delete m_CurrenAChunk;
    m_CurrenAChunk = pPreviousChunk;
}


//-----------------------------------------------------------------------------
//  This function handles all the information about the material (Texture)
//-----------------------------------------------------------------------------

void A3DSLoader::processNextMaterialChunk(A3DModel *pModel, AChunk *pPreviousChunk)
{
    int buffer[50000] = {0};
    m_CurrenAChunk = new AChunk;

    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
        readChunk(m_CurrenAChunk);

        // switch (pModel, m_CurrenAChunk->ID)
        switch (m_CurrenAChunk->ID)
        {
        case MATNAME:
            m_CurrenAChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, m_CurrenAChunk->length - m_CurrenAChunk->bytesRead, m_FilePointer);
            break;

        case MATDIFFUSE:
            readColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), m_CurrenAChunk);
            break;

        case MATMAP:
            processNextMaterialChunk(pModel, m_CurrenAChunk);
            break;

        case MATMAPFILE:
            m_CurrenAChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, m_CurrenAChunk->length - m_CurrenAChunk->bytesRead, m_FilePointer);
            break;

        default:
            m_CurrenAChunk->bytesRead += fread(buffer, 1, m_CurrenAChunk->length - m_CurrenAChunk->bytesRead, m_FilePointer);
            break;
        }
        pPreviousChunk->bytesRead += m_CurrenAChunk->bytesRead;
    }

    delete m_CurrenAChunk;
    m_CurrenAChunk = pPreviousChunk;
}

//-----------------------------------------------------------------------------
// This function reads in a chunk ID and it's length in bytes
//-----------------------------------------------------------------------------

void A3DSLoader::readChunk(AChunk *pChunk)
{
    pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

    pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

//-----------------------------------------------------------------------------
// This function reads in a string of characters
//-----------------------------------------------------------------------------

int A3DSLoader::getString(char *pBuffer)
{
    int index = 0;

    fread(pBuffer, 1, 1, m_FilePointer);

    while (*(pBuffer + index++) != 0)
    {
        fread(pBuffer + index, 1, 1, m_FilePointer);
    }

    return strlen(pBuffer) + 1;
}


//-----------------------------------------------------------------------------
// This function reads in the RGB color data
//-----------------------------------------------------------------------------

void A3DSLoader::readColorChunk(AMaterialInfo *pMaterial, AChunk *pChunk)
{
    readChunk(m_TempChunk);
    m_TempChunk->bytesRead += fread(pMaterial->color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);
    pChunk->bytesRead += m_TempChunk->bytesRead;
}


//-----------------------------------------------------------------------------
// This function reads in the indices for the vertex array
//-----------------------------------------------------------------------------

void A3DSLoader::readVertexIndices(A3DObject *pObject, AChunk *pPreviousChunk)
{
    unsigned short index = 0;
    pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

    pObject->pFaces = new AFace [pObject->numOfFaces];
    memset(pObject->pFaces, 0, sizeof(AFace) * pObject->numOfFaces);


    for(int i = 0; i < pObject->numOfFaces; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

            if(j < 3)
            {
                pObject->pFaces[i].vertIndex[j] = index;
            }
        }
    }
}


//-----------------------------------------------------------------------------
// This function reads in the UV coordinates for the object
//-----------------------------------------------------------------------------

void A3DSLoader::readUVCoordinates(A3DObject *pObject, AChunk *pPreviousChunk)
{
    pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

    pObject->pTexVerts = new AVector2 [pObject->numTexVertex];

    pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


//-----------------------------------------------------------------------------
//  This function reads in the vertices for the object
//-----------------------------------------------------------------------------

void A3DSLoader::readVertices(A3DObject *pObject, AChunk *pPreviousChunk)
{
    pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

    pObject->pVerts = new AVector3 [pObject->numOfVerts];
    memset(pObject->pVerts, 0, sizeof(AVector3) * pObject->numOfVerts);

    pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


//-----------------------------------------------------------------------------
// This function reads in the material name assigned to the object and sets the materialID
//-----------------------------------------------------------------------------

void A3DSLoader::readObjectMaterial(A3DModel *pModel, A3DObject *pObject, AChunk *pPreviousChunk)
{
    char strMaterial[255] = {0};
    int buffer[50000] = {0};

    pPreviousChunk->bytesRead += getString(strMaterial);

    for(int i = 0; i < pModel->numOfMaterials; i++)
    {
        if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
        {
            pObject->materialID = i;

            if(strlen(pModel->pMaterials[i].strFile) > 0) {

                pObject->bHasTexture = true;
            }
            break;
        }
    }

    pPreviousChunk->bytesRead += fread(buffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

AVector3 Vector(AVector3 vPoint1, AVector3 vPoint2)
{
    AVector3 vVector;

    vVector.x = vPoint1.x - vPoint2.x;
    vVector.y = vPoint1.y - vPoint2.y;
    vVector.z = vPoint1.z - vPoint2.z;

    return vVector;
}

AVector3 AddVector(AVector3 vVector1, AVector3 vVector2)
{
    AVector3 vResult;

    vResult.x = vVector2.x + vVector1.x;
    vResult.y = vVector2.y + vVector1.y;
    vResult.z = vVector2.z + vVector1.z;

    return vResult;
}

AVector3 DivideVectorByScaler(AVector3 vVector1, float Scaler)
{
    AVector3 vResult;

    vResult.x = vVector1.x / Scaler;
    vResult.y = vVector1.y / Scaler;
    vResult.z = vVector1.z / Scaler;

    return vResult;
}

//-----------------------------------------------------------------------------
// This function computes the normals and vertex normals of the objects
//-----------------------------------------------------------------------------

void A3DSLoader::computeNormals(A3DModel *pModel)
{
    AVector3 vVector1, vVector2, vNormal, vPoly[3];

    if(pModel->numOfObjects <= 0)
        return;

    for(int index = 0; index < pModel->numOfObjects; index++)
    {
        A3DObject *pObject = &(pModel->pObject[index]);

        AVector3 *pNormals      = new AVector3 [pObject->numOfFaces];
        AVector3 *pTempNormals  = new AVector3 [pObject->numOfFaces];
        pObject->pNormals       = new AVector3 [pObject->numOfVerts];

        for(int i=0; i < pObject->numOfFaces; i++)
        {
            vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
            vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
            vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];


            vVector1 = vPoly[0] - vPoly[2];
            vVector2 = vPoly[2] - vPoly[1];

            vNormal  = Cross(vVector1, vVector2);
            pTempNormals[i] = vNormal;
            vNormal  = Normalize(vNormal);

            pNormals[i] = vNormal;
        }


        AVector3 vSum(0.0, 0.0, 0.0);
        AVector3 vZero = vSum;
        int shared=0;

        for (int i = 0; i < pObject->numOfVerts; i++)
        {
            for (int j = 0; j < pObject->numOfFaces; j++)
            {
                if (pObject->pFaces[j].vertIndex[0] == i ||
                    pObject->pFaces[j].vertIndex[1] == i ||
                    pObject->pFaces[j].vertIndex[2] == i)
                {
                    vSum = vSum + pTempNormals[j];
                    shared++;
                }
            }

            pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

            pObject->pNormals[i] = Normalize(pObject->pNormals[i]);

            vSum = vZero;
            shared = 0;
        }

        delete [] pTempNormals;
        delete [] pNormals;
    }
}

} // namespace astral3d
