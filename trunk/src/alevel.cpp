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

#include "alevel.h"

using namespace std;
using namespace astral3d;

//-----------------------------------------------------------------------------
//  konstruktor
//-----------------------------------------------------------------------------

ALevel::ALevel(char *filename, char *texturePath)
{
    *this = ALevel();
    load(filename, texturePath);
}

//-----------------------------------------------------------------------------
//  konstruktor
//-----------------------------------------------------------------------------

ALevel::ALevel(Model3D *model)
{
    *this = ALevel();
    buildFromModel(model);
}

//-----------------------------------------------------------------------------
//  konstruktor
//-----------------------------------------------------------------------------

ALevel::ALevel()
{
    this->triangles = NULL;
    this->textures = NULL;
    this->listOfTriangles = NULL;
    this->numberOfTrianglesInList = NULL;
    this->colPackage.eRadius = AVector(1.0, 1.0, 1.0);
    this->gravityVector = AVector(0.0, 0.0, 0.0);
    this->gravity = false;
    this->spherePosition = AVector(0.0, 0.0, 0.0);
    this->sphereRadius = 0.0;
    this->sphere = false;
    this->textureNames = NULL;
}

//-----------------------------------------------------------------------------
// nahrava level s tim, ze textury hleda v adresari texturePath
//-----------------------------------------------------------------------------

ALevel *ALevel::load(char *filename, char *texturePath)
{
    // otevreme soubor
    ifstream file;
    file.open(filename);

    if(!file.is_open())
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::load(\""<<filename<<"\", \""<<texturePath<<"\")";
        bar << "ifstream.open("<<filename<<")";
        setAstral3DError("Can't open file with the level", foo.str(), bar.str());

        throw AReadFileException("ALevel *ALevel::load(char *filename, char *texturePath)");
    }

    // nacteni poctu textur
    file >> this->numOfTextures;

    // alokace pole pro seznam souboru s texturami
    textureNames = new string[numOfTextures];
    if(!textureNames)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::load(\""<<filename<<"\", \""<<texturePath<<"\")";
        bar << "string * ... = new string["<<numOfTextures<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        throw AMemoryAllocException("ALevel *ALevel::load(char *filename, char *texturePath)");
    }

    // alokujeme pamet pro textury
    this->textures = new GLuint[this->numOfTextures];
    if(!this->textures)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::load(\""<<filename<<"\", \""<<texturePath<<"\")";
        bar << "GLuint * ... = new GLuint["<<numOfTextures<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        throw AMemoryAllocException("ALevel *ALevel::load(char *filename, char *texturePath)");
    }

    // nacteni a vytvoreni textur levelu
    for(GLuint p=0; p<this->numOfTextures; p++)
    {
        GLuint texNumber;   // cislo textury ze souboru
        file >> texNumber;

        char texFile[256];  // soubor s texturou
        file >> texFile;

        string foo(texFile);
        this->textureNames[texNumber] = foo;

        char buffer[256];
        strcpy(buffer, texturePath);
        strcat(buffer, texFile);

        // testujeme zda-li muzeme soubor s texturou otevrit
        if(!loadTextureMipMap(buffer, &(this->textures[texNumber])))
        {
            this->destroy();

            throw ATextureException("ALevel *ALevel::load(char *filename, char *texturePath)");
        }
    }

    // nacteni poctu trojuhelniku
    file >> this->numOfTriangles;

    // alokujeme pamet pro trojuhelniky
    this->triangles = new ATriangle[this->numOfTriangles];
    if(!this->triangles)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::load(\""<<filename<<"\", \""<<texturePath<<"\")";
        bar << "ATriangle * ... = new ATriangle["<<numOfTriangles<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        this->destroy();

        throw AMemoryAllocException("ALevel *ALevel::load(char *filename, char *texturePath)");
    }

    // nacteni a vytvoreni trojuhelniku levelu
    for(GLuint p=0; p<this->numOfTriangles; p++)
    {
        // nastavime, ze trojuhelnik je validni
        this->triangles[p].valid = true;

        // nacteni identifikatoru textury
        file >> this->triangles[p].textureID;

        // nacteni bodu A a jeho texturovych koordinatu
        file >> this->triangles[p].a.x;
        file >> this->triangles[p].a.y;
        file >> this->triangles[p].a.z;
        file >> this->triangles[p].texCoordA[0];
        file >> this->triangles[p].texCoordA[1];

        // nacteni bodu B a jeho texturovych koordinatu
        file >> this->triangles[p].b.x;
        file >> this->triangles[p].b.y;
        file >> this->triangles[p].b.z;
        file >> this->triangles[p].texCoordB[0];
        file >> this->triangles[p].texCoordB[1];

        // nacteni bodu C a jeho texturovych koordinatu
        file >> this->triangles[p].c.x;
        file >> this->triangles[p].c.y;
        file >> this->triangles[p].c.z;
        file >> this->triangles[p].texCoordC[0];
        file >> this->triangles[p].texCoordC[1];

        // nacteni normaly trojuhelniku
        file >> this->triangles[p].normal.x;
        file >> this->triangles[p].normal.y;
        file >> this->triangles[p].normal.z;
    }

    file.close();

    // finally we create triangle lists
    if (!createLists())
    {
        throw AException("ALevel *ALevel::load(char *filename, char *texturePath)");
    }

    return this;
}

//-----------------------------------------------------------------------------
// odstrani trojuhelnik
//-----------------------------------------------------------------------------

bool ALevel::removeTriangle(GLuint id)
{
    // odstrani trojuhelnik ze seznamu trojuhelniku, nikoli v pameti
    // a ze seznamu triangles

    // hledany seznam, ve kterem se nachazi ruseny trojuhelnik
    int foo = -1;

    for(GLuint p=0; p<numOfTextures; p++)
    {
        for(GLuint q=0; q<numberOfTrianglesInList[p]; q++)
        {
            // nasli jsme seznam, ve kterem je ruseny trojuhelnik
            if(listOfTriangles[p][q] == id)
                foo = (int) p;
        }
    }

    // trojuhelnik neni v seznamu
    if(foo == -1)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::removeTriangle("<<id<<")";
        bar << "N/A";
        setAstral3DError("Triangles ID doesn't exist", foo.str(), bar.str());
        return false;
    }

    // novy seznam
    GLuint *bar = new GLuint[numberOfTrianglesInList[foo]-1];
    if(!bar)
    {
        GLuint pom = numberOfTrianglesInList[foo]-1;
        stringstream foo;
        stringstream bar;
        foo << "ALevel::removeTriangle("<<id<<")";
        bar << "GLuint * ... = new GLuint["<<pom<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());
        return false;
    }

    // kopirujeme seznam
    int q=0;
    for(GLuint p=0; p<numberOfTrianglesInList[foo]; p++)
    {
        if(listOfTriangles[foo][p] != id)
        {
            bar[q]=listOfTriangles[foo][p];
            q++;
        }
    }

    // smazeme stary seznam
    delete[] listOfTriangles[foo];

    // nastavime seznam novy
    listOfTriangles[foo] = bar;

    // snizime pocet trojuhelniku v seznamu
    numberOfTrianglesInList[foo]--;

    // trojuhelnik neni validni (pri ukladani - metoda save - se neulozi)
    this->triangles[id].valid = false;


    return true;
}

//-----------------------------------------------------------------------------
// prida trojuhelnik do levelu
//-----------------------------------------------------------------------------

bool ALevel::addTriangle(ATriangle triangle)
{
    // pokud se nejedna o validni trojuhelnik
    if(!triangle.valid)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::addTriangle(ATriangle)";
        bar << "N/A";
        setAstral3DError("Triangle isn't valid. Set triangles validity to 'true'", foo.str(), bar.str());

        return false;
    }

    // pokud nezname ID textury, kterou ma trojuhelnik nastaven, vratime false
    if(triangle.textureID < 0 || triangle.textureID >= numOfTextures)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::addTriangle(ATriangle)";
        bar << "0 <= " << triangle.textureID << " < " << numOfTextures;
        setAstral3DError("Triangles texture ID isn't known", foo.str(), bar.str());

        return false;
    }

    // pridame trojuhelnik k ostatnim
    ATriangle *foo = new ATriangle[numOfTriangles + 1];
    if(!foo)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::addTriangle(ATriangle)";
        bar << "ATriangle * ... = new ATriangle["<<numOfTriangles + 1<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        return false;
    }

    for(GLuint p=0; p<numOfTriangles; p++)
    {
        foo[p] = triangles[p];
    }

    // nakonec pridame nas novy trojuhelnik
    foo[numOfTriangles] = triangle;

    // zvysime pocet trojuhelniku o jeden
    numOfTriangles++;

    // uvolnime starou pamet
    if(this->triangles)
    {
        delete [] this->triangles;
        this->triangles = NULL;
    }

    // a nastavime novy seznam trojuhelniku
    this->triangles = foo;

    // pocet trojuhelniku majici stejnou texturu
    GLuint count = numberOfTrianglesInList[triangle.textureID];

    // alokujeme pamet pro vytvoreni noveho seznamu
    GLuint *bar = new GLuint[count + 1];
    if(!bar)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::addTriangle(ATriangle)";
        bar << "GLuint * ... = new GLuint["<<count+1<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        return false;
    }

    // kopirujeme seznam
    for(GLuint p=0; p<count; p++)
    {
        bar[p] = listOfTriangles[triangle.textureID][p];
    }

    bar[count] = numOfTriangles-1;

    // uvolnime starou pamet
    if(this->listOfTriangles[triangle.textureID])
    {
        delete [] this->listOfTriangles[triangle.textureID];
        this->listOfTriangles[triangle.textureID] = NULL;
    }

    // a nastavime novy seznam
    this->listOfTriangles[triangle.textureID] = bar;

    // nakonec zvysime pocet trojuhelniku v tomto seznamu na citaci o jednicku
    numberOfTrianglesInList[triangle.textureID]++;

    return true;
}

//-----------------------------------------------------------------------------
// ulozi informace o seznamu trojuhelniku podle textur
//-----------------------------------------------------------------------------

void ALevel::saveListOfTriangles(char *filename)
{
    ofstream file;
    file.open(filename);

    if(!file.is_open())
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::saveListOfTriangles(\""<<filename<<"\")";
        bar << "ofstream.open(" << filename << ")";
        setAstral3DError("Can't open file", foo.str(), bar.str());

        throw AWriteFileException("void ALevel::saveListOfTriangles(char *filename)");
    }

    // pro kazdou texturu ulozime seznam trojuhelniku, ktere ji obsahuji
    for(GLuint p=0; p<this->numOfTextures; p++)
    {
        file << "texture ID = " << p;
        file << ", triangle count = " << this->numberOfTrianglesInList[p];
        file << endl;

        for(GLuint q=0; q<this->numberOfTrianglesInList[p]; q++)
            file << this->listOfTriangles[p][q] << ",";


        file << endl << endl;
    }
}

//-----------------------------------------------------------------------------
// ulozi level do souboru
//-----------------------------------------------------------------------------

void ALevel::save(char *filename)
{
    ofstream file;
    file.open(filename);

    if(!file.is_open())
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::save(\""<<filename<<"\")";
        bar << "ofstream.open(\"" << filename << "\")";
        setAstral3DError("Can't open file", foo.str(), bar.str());

        throw AWriteFileException("void ALevel::save(char *filename)");
    }

    // ulozime pocet textur
    file << this->numOfTextures << endl << endl;

    // ulozime textury
    for(GLuint p=0; p<this->numOfTextures; p++)
    {
        file << p << " " << textureNames[p] << endl;
    }

    file << endl;


    // ulozime pocet validnich trojuhelniku
    GLuint count=0;
    for(GLuint p=0; p<this->numOfTriangles; p++)
    {
        if(this->triangles[p].valid)
            count++;
    }

    file << count << endl << endl;

    // postupne ukladame vsechny trojuhelniky
    for(GLuint p=0; p<this->numOfTriangles; p++)
    {
        // ukladame pouze validni trojuhelniky
        if(this->triangles[p].valid)
        {
        file << this->triangles[p].textureID << endl;

        // ulozeni bodu A a jeho texturovych koordinatu
        file << this->triangles[p].a.x << " ";
        file << this->triangles[p].a.y << " ";
        file << this->triangles[p].a.z << " ";
        file << this->triangles[p].texCoordA[0] << " ";
        file << this->triangles[p].texCoordA[1] << endl;

        // ulozeni bodu B a jeho texturovych koordinatu
        file << this->triangles[p].b.x << " ";
        file << this->triangles[p].b.y << " ";
        file << this->triangles[p].b.z << " ";
        file << this->triangles[p].texCoordB[0] << " ";
        file << this->triangles[p].texCoordB[1] << endl;

        // ulozeni bodu C a jeho texturovych koordinatu
        file << this->triangles[p].c.x << " ";
        file << this->triangles[p].c.y << " ";
        file << this->triangles[p].c.z << " ";
        file << this->triangles[p].texCoordC[0] << " ";
        file << this->triangles[p].texCoordC[1] << endl;

        // ulozeni normaly trojuhelniku
        file << this->triangles[p].normal.x << " ";
        file << this->triangles[p].normal.y << " ";
        file << this->triangles[p].normal.z << endl;

        file << endl;
        } // if(this->triangles[p].valid)
    }

    file.close();
}

//-----------------------------------------------------------------------------
// rozseka trojuhelniky na mensi nepresahujici dany obsah
//-----------------------------------------------------------------------------

/*
Tato metoda rozseka prilis velke trojuhelniky na mensi a to tak, aby jejich
obsah byl mensi nez je udan v prvnim parametru. Princip je takovy, ze se
najde trojuhelnik s velkym obsahem, ktery je zaroven validni. Takovy
trojuhelnik ma jiste nejdelsi stranu - odvesnu. Najde se stredovy bod odvesny
a v tomto stredovem bodu dojde k rozdeleni velkeho trojuhelniku na dva mensi.
Pro stredovy bod odvesny se prepocitaji texturove koordinaty a oba nove
vznikle trojuhelniky se pridaji do levelu. Stary, velky, trojuhelnik je
odstranen.

              C
            / | \
           /  |   \
          /    |    \
         /     |      \
        /      |        \
       /        |         \
      /         |           \
     /           |            \
    /            |              \
   /              |               \
  ---------------------------------
  A               S                 B

*/

void ALevel::splitTriangles(double s, bool recursive)
{
    GLuint loop = numOfTriangles;

    bool needRecursion = false;

    for(GLuint p=0; p<loop; p++)
    {
        // trojuhelnik musi vyt validni
        if(!triangles[p].valid)
            continue;

        // strany trojuhelniku
        AVector a = triangles[p].b - triangles[p].c;
        AVector b = triangles[p].a - triangles[p].c;
        AVector c = triangles[p].a - triangles[p].b;

        double foo = abs(a % b) / 2;

        // trojuhelnik ma prilis velky obsah, musime jej rozdelit na dva
        if(foo > s)
        {
            // nasli jsme trojuhelnik, co ma prilis velky obsah,
            // nastavime needRecursion na true a zkusime jeste priste
            // rozdeli (zanorujeme rekurzi)
            needRecursion = true;

            // najdeme odvesnu (nejdelsi strana) a jeji stred
            char odvesna;

            if(a.getLength() > b.getLength())
            {
                if(a.getLength() > c.getLength())
                    odvesna = 'a';
                else
                    odvesna = 'c';
            }
            else
            {
                if(b.getLength() > c.getLength())
                    odvesna = 'b';
                else
                    odvesna = 'c';
            }

            // Nove vytvorene trojuhelniky
            ATriangle tr1, tr2;
            AVector stred;
            // souradnice textury stredoveho bodu na odvesne
            double u, v;

            tr1 = triangles[p];
            tr2 = triangles[p];

            switch(odvesna)
            {
                // urcime stred odvesny a podle nej vytvorime dva nove
                // trojuhelniky
                case 'a':
                    stred = (triangles[p].b + triangles[p].c) * 0.5;
                    u = (triangles[p].texCoordB[0] + triangles[p].texCoordC[0]) * 0.5;
                    v = (triangles[p].texCoordB[1] + triangles[p].texCoordC[1]) * 0.5;

                    tr1.c = stred;
                    tr1.texCoordC[0] = u;
                    tr1.texCoordC[1] = v;

                    tr2.b = stred;
                    tr2.texCoordB[0] = u;
                    tr2.texCoordB[1] = v;
                    break;

                case 'b':
                    stred = (triangles[p].a + triangles[p].c) * 0.5;
                    u = (triangles[p].texCoordA[0] + triangles[p].texCoordC[0]) * 0.5;
                    v = (triangles[p].texCoordA[1] + triangles[p].texCoordC[1]) * 0.5;

                    tr1.c = stred;
                    tr1.texCoordC[0] = u;
                    tr1.texCoordC[1] = v;

                    tr2.a = stred;
                    tr2.texCoordA[0] = u;
                    tr2.texCoordA[1] = v;
                    break;

                case 'c':
                    stred = (triangles[p].a + triangles[p].b) * 0.5;

                    u = (triangles[p].texCoordA[0] + triangles[p].texCoordB[0]) * 0.5;
                    v = (triangles[p].texCoordA[1] + triangles[p].texCoordB[1]) * 0.5;

                    tr1.b = stred;
                    tr1.texCoordB[0] = u;
                    tr1.texCoordB[1] = v;

                    tr2.a = stred;
                    tr2.texCoordA[0] = u;
                    tr2.texCoordA[1] = v;
                    break;
            }

            this->removeTriangle(p);
            this->addTriangle(tr1);
            this->addTriangle(tr2);

        } // if(foo > s)
    }

    if(recursive && needRecursion)
        splitTriangles(s, recursive);
}

//-----------------------------------------------------------------------------
// renderuje level
//-----------------------------------------------------------------------------

void ALevel::render()
{
    /* vzdy vykreslujeme vsechny trojuhelniky pro prislusnou texturu */

    for(GLuint p=0; p<this->numOfTextures; p++)
    {
        // vybereme danou texturu
        glBindTexture(GL_TEXTURE_2D, this->textures[p]);

        glBegin(GL_TRIANGLES);

        // prochazime seznam trojuhelniku majici tuto texturu
        for(GLuint q=0; q<this->numberOfTrianglesInList[p]; q++)
        {
            GLuint t = listOfTriangles[p][q];

            double v[3];

            // nastaveni normaly
            v[0] = this->triangles[t].normal.x;
            v[1] = this->triangles[t].normal.y;
            v[2] = this->triangles[t].normal.z;
            glNormal3dv(v);

            // nastaveni a vykresleni bodu A
            glTexCoord2dv(this->triangles[t].texCoordA);
            v[0] = this->triangles[t].a.x;
            v[1] = this->triangles[t].a.y;
            v[2] = this->triangles[t].a.z;
            glVertex3dv(v);

            // nastaveni a vykresleni bodu B
            glTexCoord2dv(this->triangles[t].texCoordB);
            v[0] = this->triangles[t].b.x;
            v[1] = this->triangles[t].b.y;
            v[2] = this->triangles[t].b.z;
            glVertex3dv(v);

            // nastaveni a vykresleni bodu C
            glTexCoord2dv(this->triangles[t].texCoordC);
            v[0] = this->triangles[t].c.x;
            v[1] = this->triangles[t].c.y;
            v[2] = this->triangles[t].c.z;
            glVertex3dv(v);
        }

        glEnd();
    }
}

//-----------------------------------------------------------------------------
// Uvolnuje pamet pouzitou pro level
//-----------------------------------------------------------------------------

void ALevel::destroy()
{
    if(this->textureNames)
        delete[] this->textureNames;

    if(this->textures)
    {
        // uvolneni textur z pameti
        for(GLuint p=0; p<this->numOfTextures; p++)
            deleteTexture(&(this->textures[p]));

        delete [] this->textures;
    }

    // pokud mame seznam seznamu trojuhelniku
    if(this->listOfTriangles)
    {
        // prochazime ho a rusime jednotlive seznamy
        for(GLuint p=0; p<this->numOfTextures; p++)
            if(this->listOfTriangles[p])
                delete [] this->listOfTriangles[p];

        // nakonec ho smazem cely
        delete [] this->listOfTriangles;
    }

    if(this->triangles)
        delete [] this->triangles;

    if(this->numberOfTrianglesInList)
        delete [] this->numberOfTrianglesInList;

    this->triangles = NULL;
    this->textures = NULL;
    this->listOfTriangles = NULL;
    this->numberOfTrianglesInList = NULL;
    this->textureNames = NULL;
}

//-----------------------------------------------------------------------------
// Vraci novy vektor pohybu v zavislosti na kolizi a slidingu
//-----------------------------------------------------------------------------

AVector ALevel::getDirection(const AVector &pos, const AVector &vel)
{
    AVector newPos = this->getPosition(pos, vel);
    return newPos-pos;
}

//-----------------------------------------------------------------------------
// Vraci novy vektor pohybu v zavislosti na kolizi a slidingu a gravitaci
//-----------------------------------------------------------------------------

AVector ALevel::getGravityDirection(const AVector &pos)
{
    return this->getDirection(pos, this->gravityVector);
}

//-----------------------------------------------------------------------------
// Vraci novou pozici v zavislosti na kolizi a slidingu a gravitaci
//-----------------------------------------------------------------------------

AVector ALevel::getGravityPosition(const AVector &pos)
{
    return this->getPosition(pos, this->gravityVector);
}

//-----------------------------------------------------------------------------
// Vraci novou pozici v zavislosti na kolizi a slidingu
//-----------------------------------------------------------------------------

AVector ALevel::getPosition(const AVector &pos, const AVector &vel)
{
    // nastaveni parametru kolizni struktury
    colPackage.r3Position = pos;
    colPackage.r3Velocity = vel;

    // nastaveni pozice ve vektorovem prostoru kolizniho elipsoidu
    AVector eSpacePosition;
    eSpacePosition.x = colPackage.r3Position.x / colPackage.eRadius.x;
    eSpacePosition.y = colPackage.r3Position.y / colPackage.eRadius.y;
    eSpacePosition.z = colPackage.r3Position.z / colPackage.eRadius.z;

    // nastaveni pohybu ve vektorovem prostoru kolizniho elipsoidu
    AVector eSpaceVelocity;
    eSpaceVelocity.x = colPackage.r3Velocity.x / colPackage.eRadius.x;
    eSpaceVelocity.y = colPackage.r3Velocity.y / colPackage.eRadius.y;
    eSpaceVelocity.z = colPackage.r3Velocity.z / colPackage.eRadius.z;

    // rekurzivni volani kolize
    collisionRecursionDepth = 0;
    AVector finalPosition = collideWithWorld(eSpacePosition, eSpaceVelocity);

    // nastavime zpatky do naseho vektoroveho prostoru
    finalPosition.x = finalPosition.x * colPackage.eRadius.x;
    finalPosition.y = finalPosition.y * colPackage.eRadius.y;
    finalPosition.z = finalPosition.z * colPackage.eRadius.z;

    // vratime vysledny vektor podel ktereho se musime pohybovat
    return finalPosition;
}

//-----------------------------------------------------------------------------
// provadi kolizi s levelem ve vektorovem prostoru kolizniho
// elipsoidu a vola ji rekurzivne
//-----------------------------------------------------------------------------

AVector ALevel::collideWithWorld(const AVector &pos, const AVector &vel)
{
    // nesmime se moc rekurzivne zanorovat
    if (collisionRecursionDepth>5)
        return pos;

    // nastaveni informaci pro pohyb
    colPackage.velocity = vel;
    colPackage.normalizedVelocity = vel;
    colPackage.normalizedVelocity.normalize();
    colPackage.basePoint = pos;
    colPackage.foundCollision = false;

    // konstrola kolize se vsemi trojuhelniky tvoricimi level
    this->checkCollision();

    AVector newPos, newVel;

    // vypocet slidingu
    if(slide(&colPackage, 100.0, &newPos, &newVel))
    {
        // pokud je to potreba, provadime dalsi pohyb s kolizi
        collisionRecursionDepth++;
        return collideWithWorld(newPos, newVel);
    }
    else
    {
        // znamena, ze jsme nemuseli provadet sliding a pouze posuneme
        // na vyslednou pozici
        return newPos;
    }

}

//-----------------------------------------------------------------------------
// testuje kolizi proti trojuhelnikum levelu
//-----------------------------------------------------------------------------

void ALevel::checkCollision()
{
    // pokud neni zapnuta kolize s trojuhelniky pouze v dane sfere
    if(!this->sphere)
    {
        for(GLuint p=0; p<this->numOfTriangles; p++)
        {
            checkTriangle(&colPackage, triangles[p].a, triangles[p].b, triangles[p].c, triangles[p].normal);
        }
    }
    // pokud je zapnuta kolize s trojuhelniky pouze v dane sfere
    else // if(this->sphere)
    {

    AVector sPosition = this->spherePosition;
    double sRadius = this->sphereRadius;

    AVector length;

    // porovnavame druhe mocniny => rychlejsi vypocet
    sRadius *= sRadius;

    for(GLuint p=0; p<this->numOfTriangles; p++)
    {
        length = sPosition - triangles[p].a;
        if(length.squaredLength() < sRadius)
        {
            length = sPosition - triangles[p].b;
            if(length.squaredLength() < sRadius)
            {
                length = sPosition - triangles[p].c;
                if(length.squaredLength() < sRadius)
                {
                    checkTriangle(&colPackage, triangles[p].a, triangles[p].b, triangles[p].c, triangles[p].normal);
                }
            }
        }
    }
    } // if(this->sphere)
}

//-----------------------------------------------------------------------------
// builds the level from 3DS file being loaded as the A3DSModel class
//-----------------------------------------------------------------------------

ALevel *ALevel::buildFromModel(Model3D *model)
{
    // if the level allready exists we destroy it
    destroy();

    // we need 3D model containing model data
    A3DModel *pModel = model->get3DModel();
    if(!pModel)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::buildFromModel("<<model<<")";
        bar << "A3DModel::get3DModel()";
        setAstral3DError("Can't get pointer to A3DModel class", foo.str(), bar.str());

        throw AMemoryAllocException("ALevel *ALevel::buildFromModel(Model3D *model)");
    }

    // first we find total count of textures
    GLuint textureCount = 0;
    for(int i = 0; i < pModel->numOfObjects; i++)
    {
        if(pModel->pObject.size() <= 0)
            break;

        A3DObject *pObject = &pModel->pObject[i];

        // if the object has the texture
        if(pObject->bHasTexture)
        {
            textureCount++;
        }
    }

    // we create texture array
    numOfTextures = textureCount;
    textures = new GLuint[numOfTextures];
    if(!textures)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::buildFromModel("<<model<<")";
        bar << "GLuint * ... = new GLuint["<<numOfTextures<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        destroy();

        throw AMemoryAllocException("ALevel *ALevel::buildFromModel(Model3D *model)");
    }

    // we create array for texture names
    textureNames = new string[numOfTextures];
    if(!textureNames)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::buildFromModel("<<model<<")";
        bar << "string * ... = new string["<<numOfTextures<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        destroy();

        throw AMemoryAllocException("ALevel *ALevel::buildFromModel(Model3D *model)");
    }

    // than we load textures
    for(int i = 0; i < pModel->numOfObjects; i++)
    {
        if(pModel->pObject.size() <= 0)
            break;

        A3DObject *pObject = &pModel->pObject[i];

        // if the object has the texture we load it
        if(pObject->bHasTexture)
        {
            int ID = pObject->materialID;
            // texture filename
            string foo(pModel->pMaterials[ID].strFile);
            textureNames[ID] = foo;

            char buf[256];
            // path to the directory where all textures are located
            string bar(model->getTexturePath());
            strcpy(buf, bar.c_str());
            strcat(buf, textureNames[ID].c_str());

            // loads the texture
            if(!loadTextureMipMap(buf, &(textures[ID])))
            {
                this->destroy();
                throw ATextureException("ALevel *ALevel::buildFromModel(Model3D *model)");
            }
        }
    }

    // we find total count of triangles
    GLuint triangleCount = 0;
    for(int i = 0; i < pModel->numOfObjects; i++)
    {
        if(pModel->pObject.size() <= 0)
            break;

        A3DObject *pObject = &pModel->pObject[i];

        // one face = one triangle
        triangleCount += pObject->numOfFaces;
    }

    numOfTriangles = triangleCount;

    // we create triangle array
    triangles = new ATriangle[numOfTriangles];
    if(!triangles)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::buildFromModel("<<model<<")";
        bar << "ATriangle * ... = new ATriangle["<<numOfTriangles<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        destroy();

        throw AMemoryAllocException("ALevel *ALevel::buildFromModel(Model3D *model)");
    }

    // now we load all triangles
    int p=0;
    for(int i = 0; i < pModel->numOfObjects; i++)
    {
        if(pModel->pObject.size() <= 0)
            break;

        A3DObject *pObject = &pModel->pObject[i];

        // we add all object faces - triangles
        for(int j = 0; j < pObject->numOfFaces; j++)
        {

            int index_a = pObject->pFaces[j].vertIndex[0];
            int index_b = pObject->pFaces[j].vertIndex[1];
            int index_c = pObject->pFaces[j].vertIndex[2];

            // triangle is valid
            triangles[p].valid = true;
            // triangles texture
            triangles[p].textureID = pObject->materialID;

            // vertex A
            triangles[p].a.x = pObject->pVerts[ index_a ].x;
            triangles[p].a.y = pObject->pVerts[ index_a ].y;
            triangles[p].a.z = pObject->pVerts[ index_a ].z;
            triangles[p].texCoordA[0] = pObject->pTexVerts[ index_a ].x;
            triangles[p].texCoordA[1] = pObject->pTexVerts[ index_a ].y;

            // vertex B
            triangles[p].b.x = pObject->pVerts[ index_b ].x;
            triangles[p].b.y = pObject->pVerts[ index_b ].y;
            triangles[p].b.z = pObject->pVerts[ index_b ].z;
            triangles[p].texCoordB[0] = pObject->pTexVerts[ index_b ].x;
            triangles[p].texCoordB[1] = pObject->pTexVerts[ index_b ].y;

            // vertex C
            triangles[p].c.x = pObject->pVerts[ index_c ].x;
            triangles[p].c.y = pObject->pVerts[ index_c ].y;
            triangles[p].c.z = pObject->pVerts[ index_c ].z;
            triangles[p].texCoordC[0] = pObject->pTexVerts[ index_c ].x;
            triangles[p].texCoordC[1] = pObject->pTexVerts[ index_c ].y;

            // normal
            triangles[p].normal.x = pObject->pNormals[ index_a ].x;
            triangles[p].normal.y = pObject->pNormals[ index_a ].y;
            triangles[p].normal.z = pObject->pNormals[ index_a ].z;

            // triangle array index
            p++;
        }
    }

    if (!createLists())
    {
        throw AException("ALevel *ALevel::buildFromModel(Model3D *model)");
    }

    return this;
}

//-----------------------------------------------------------------------------
// creates lists of triangles according to the textures
//-----------------------------------------------------------------------------

bool ALevel::createLists()
{
    // vytvorime seznam odkazu na trojuhelniky
    this->numberOfTrianglesInList = new GLuint[this->numOfTextures];
    if(!this->numberOfTrianglesInList)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::createLists()";
        bar << "GLuint * ... = new GLuint["<<numOfTextures<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        this->destroy();
        return false;
    }

    // vytvorime seznam seznamu pro trojuhelniky
    this->listOfTriangles  = new pGLuint[this->numOfTextures];
    if(!this->listOfTriangles)
    {
        stringstream foo;
        stringstream bar;
        foo << "ALevel::createLists()";
        bar << "GLuint * ... = new GLuint["<<numOfTextures<<"]";
        setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

        this->destroy();
        return false;
    }

    // vytvorime seznam jednotlivych trojuhelniku podle textur
    for(GLuint p=0; p<this->numOfTextures; p++)
    {
        GLuint count = 0;
        // spocitame kolik trojuhelniku ma texturu rovnou p
        for(GLuint q=0; q<this->numOfTriangles; q++)
        {
            if(this->triangles[q].textureID == p)
                count++;
        }

        // vytvorime seznam pro prave spocitany pocet trojuhelniku
        this->listOfTriangles[p] = NULL;
        this->listOfTriangles[p] = new GLuint[count];
        if(!this->listOfTriangles[p])
        {
            stringstream foo;
            stringstream bar;
            foo << "ALevel::createLists()";
            bar << "GLuint * ... = new GLuint["<<count<<"]";
            setAstral3DError("Can't allocate memory: operator 'new' failed", foo.str(), bar.str());

            this->destroy();
            return false;
        }

        this->numberOfTrianglesInList[p] = count;

        // a priradime trojuhelniky do nove vytvoreneho seznamu
        GLuint u = 0;
        for(GLuint q=0; q<this->numOfTriangles; q++)
        {
            if(this->triangles[q].textureID == p)
            {
                this->listOfTriangles[p][u] = q;
                u++;
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// applies the OpenGL matrix to the level
//-----------------------------------------------------------------------------

void ALevel::applyMatrix(double mat[16])
{
    for (GLuint p = 0; p < numOfTriangles; p++)
    {
        triangles[p].a.applyMatrix(mat);
        triangles[p].b.applyMatrix(mat);
        triangles[p].c.applyMatrix(mat);
        triangles[p].normal.applyMatrix(mat);
    }
}
