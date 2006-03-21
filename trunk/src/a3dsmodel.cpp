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

#include "a3dsmodel.h"

using namespace std;
namespace astral3d {

//-----------------------------------------------------------------------------
// This method loads the 3DS file and creates all textures
//-----------------------------------------------------------------------------

bool A3DSModel::load(char* filename, char *texturePath)
{

#ifdef DEBUG
    cout << "vstupuji do metody load tridy A3DSModel" << endl;
    cout << "vytvarim novy A3DModel a A3DSLoader" << endl;
#endif

    // we create new model
    m3DModel = new A3DModel;
    if(!m3DModel)
    {
        stringstream foo;
        stringstream bar;
        foo << "A3DSModel::load(\""<<filename<<"\", "<<texturePath<<")";
        bar << "A3DModel * ... = new A3DModel";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());
        return false;
    }

    // and new model loader
    mLoad3ds = new A3DSLoader;
    if(!mLoad3ds)
    {
        stringstream foo;
        stringstream bar;
        foo << "A3DSModel::load(\""<<filename<<"\", "<<texturePath<<")";
        bar << "A3DSLoader * ... = new A3DSLoader";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());
        return false;
    }

#ifdef DEBUG
    cout << "vytvoreno" << endl;
    cout << "nacitam model pomoci metody load3DSModel tridy A3DSLoader" << endl;
#endif

    // model loading
    bool foo = mLoad3ds->load3DSModel(m3DModel, filename);
    if(foo == false)
    {
        delete mLoad3ds;
        delete m3DModel;
        return false;
    }

#ifdef DEBUG
    cout << "nacteno" << endl;
    cout << "nacitam textury modelu" << endl;
#endif

    // save texture path
    string bar(texturePath);
    this->texturePath = bar;

    // model is loaded we don't need this any more
    delete mLoad3ds;

    // we have to take care of the textures and load them
    for(int i=0; i<m3DModel->numOfMaterials; i++)
    {
        if(strlen(m3DModel->pMaterials[i].strFile) > 0)
        {
            char buf[256];
            strcpy(buf, texturePath);
            strcat(buf, m3DModel->pMaterials[i].strFile);

            bool foo = loadTextureMipMap(buf, &TextureArray3ds[i]);
            if(foo == false)
            {
                destroy();
                return false;
            }
        }
        m3DModel->pMaterials[i].texureId = i;
    }

#ifdef DEBUG
    cout << "textury nacteny" << endl;
#endif

    return true;
}

//-----------------------------------------------------------------------------
// This method draws the model on the screen
//-----------------------------------------------------------------------------

void A3DSModel::render()
{
    // render all objects building the model
    for(int i = 0; i < m3DModel->numOfObjects; i++)
    {

        if(m3DModel->pObject.size() <= 0)
            break;

        A3DObject *pObject = &m3DModel->pObject[i];

        // if the object has the texture we use it
        if(pObject->bHasTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glColor3ub(255, 255, 255);
            glBindTexture(GL_TEXTURE_2D, TextureArray3ds[pObject->materialID]);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
            glColor3ub(255, 255, 255);
        }

        // triangles are building the object
        glBegin(GL_TRIANGLES);

        for(int j = 0; j < pObject->numOfFaces; j++)
        {
            // draws one face - triangle
            for(int whichVertex = 0; whichVertex < 3; whichVertex++)
            {
                // find the index of the vertex
                int index = pObject->pFaces[j].vertIndex[whichVertex];

                // sets the vertex normal
                glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);

                // if the object has the texture we set it
                if(pObject->bHasTexture)
                {
                    // vertex needs to have the texture coordinates to be able to use the texture
                    if(pObject->pTexVerts)
                    {
                        glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
                    }
                }
                else
                {
                    // object hasn't the texture we use color if we are able to
                    if((int)m3DModel->pMaterials.size() < pObject->materialID)
                    {
                        int *pColor = m3DModel->pMaterials[pObject->materialID].color;
                        glColor3ub(pColor[0], pColor[1], pColor[2]);
                    }
                }
                // finally we draw the vertex
                glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
            }
        }
        glEnd();
    }
}

//-----------------------------------------------------------------------------
// This method frees the memory
//-----------------------------------------------------------------------------

void A3DSModel::destroy()
{
    if(m3DModel == NULL)
        return;

    for(int i = 0; i < m3DModel->numOfObjects; i++)
    {
        delete [] m3DModel->pObject[i].pFaces;
        delete [] m3DModel->pObject[i].pNormals;
        delete [] m3DModel->pObject[i].pVerts;
        delete [] m3DModel->pObject[i].pTexVerts;
    }

    delete m3DModel;
    m3DModel = NULL;
}

} // namespace astral3d
