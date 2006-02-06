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

#include "atexture.h"

using namespace std;
namespace astral3d {

//-----------------------------------------------------------------------------
// pomocna funkce pro urceni typu grafickeho souboru
//-----------------------------------------------------------------------------

int fileType(char *filename)
{
    // podle pripony souboru zjisti zda-li se jedna o BMP, TGA, JPG nebo PNG

    // pokud je nazev souboru kratsi nez tri pismena, vrati se implicitni BMP
    if(strlen(filename) < 3)
        return BMP;

    char foo[4];

    foo[0] = tolower(filename[strlen(filename)-3]);
    foo[1] = tolower(filename[strlen(filename)-2]);
    foo[2] = tolower(filename[strlen(filename)-1]);
    foo[3] = '\0';

    if(strcmp(foo, "jpg") == 0) return JPG;
    if(strcmp(foo, "jpeg") == 0) return JPG;
    if(strcmp(foo, "png") == 0) return PNG;
    if(strcmp(foo, "tga") == 0) return TGA;
    if(strcmp(foo, "bmp") == 0) return BMP;

    // implicitne se vraci NA
    return NA;
}

//-----------------------------------------------------------------------------
// pomocna funkce pro transformaci pixelu pro formaty BMP a TGA
//-----------------------------------------------------------------------------

unsigned char *transform(SDL_Surface *surface)
{
    unsigned int len = surface->w * surface->h * 3;
    int i, j, k, l = 0;

    unsigned char *tmp = new unsigned char[len];
    if(tmp == NULL)
        return NULL;

    unsigned char *foo = (unsigned char*)surface->pixels;

    memcpy(tmp, surface->pixels, len);


    for (i = surface->w-1; i >=0; i--)
    {
        for(j = 0; j < surface->h; j++)
        {
            for (k = 2; k >= 0; k-- )
            {
                tmp[l++] = foo[i * surface->h * 3 + j * 3 + k];
            }
        }
    }

    return tmp;
}

//-----------------------------------------------------------------------------
// nacteni BMP souboru jako OpenGL textury
//-----------------------------------------------------------------------------

bool loadTexture(char *filename, GLuint *texture, GLint min, GLint mag)
{
    SDL_Surface *Image = NULL;

    if((Image = IMG_Load(filename)))
    {
        int type = fileType(filename);
        return createTexture(Image, texture, min, mag, type);
    }

    if (Image)
        SDL_FreeSurface(Image);

    stringstream foo;
    stringstream bar;
    foo << "loadTexture(\""<<filename<<"\", "<<texture<<", "<<min<<", "<<mag<<")";
    bar << "IMG_Load("<<filename<<")";
    setAstral3DError("Can't open file with the texture", foo.str(), bar.str());

    return false;
}

//-----------------------------------------------------------------------------
// vytvoreni textury z SDL povrchu
//-----------------------------------------------------------------------------

bool createTexture(SDL_Surface *Image, GLuint *texture, GLint min, GLint mag, int type)
{
    if(Image == NULL)
    {
        stringstream foo;
        stringstream bar;
        foo << "createTexture("<<Image<<", "<<texture<<", "<<min<<", "<<mag<<", "<<type<<")";
        bar << "SDL_Surface is NULL";
        setAstral3DError("Can't read SDL_Surface structure", foo.str(), bar.str());
        return false;
    }


    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    unsigned char *data;

    if(type == BMP || type == TGA)
    {
        data = transform(Image);
        if(data == NULL)
        {
            stringstream foo;
            stringstream bar;
            foo << "createTexture("<<Image<<", "<<texture<<", "<<min<<", "<<mag<<", "<<type<<")";
            bar << "transform("<<Image<<")";
            setAstral3DError("Can't transform pixel data for BMP/TGA texture", foo.str(), bar.str());
            return false;
        }
    }
    else if(type == JPG || type == PNG)
    {
        data = (unsigned char*)Image->pixels;
    }
    else
    {
        stringstream foo;
        stringstream bar;
        foo << "createTexture("<<Image<<", "<<texture<<", "<<min<<", "<<mag<<", "<<type<<")";
        bar << "N/A";
        setAstral3DError("Unknown texture type (BMP, TGA, JPG and PNG are allowed)", foo.str(), bar.str());
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, 3, Image->w,Image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

    delete data;

    return true;
}

//-----------------------------------------------------------------------------
// nacteni BMP souboru jako OpenGL textury
//-----------------------------------------------------------------------------

bool loadTextureMipMap(char *filename, GLuint *texture)
{
    SDL_Surface *Image = NULL;

    if((Image = IMG_Load(filename)))
    {
        int type = fileType(filename);
        return createTextureMipMap(Image, texture, type);
    }

    if (Image)
        SDL_FreeSurface(Image);

    stringstream foo;
    stringstream bar;
    foo << "loadTextureMipMap(\""<<filename<<"\", "<<texture<<")";
    bar << "IMG_Load("<<filename<<")";
    setAstral3DError("Can't open file with the texture", foo.str(), bar.str());

    return false;
}

//-----------------------------------------------------------------------------
// vytvoreni textury z SDL povrchu
//-----------------------------------------------------------------------------

bool createTextureMipMap(SDL_Surface *Image, GLuint *texture, int type)
{
    if(Image == NULL)
    {
        stringstream foo;
        stringstream bar;
        foo << "createTextureMipMap("<<Image<<", "<<texture<<", "<<type<<")";
        bar << "SDL_Surface is NULL";
        setAstral3DError("Can't read SDL_Surface structure", foo.str(), bar.str());
        return false;
        return false;
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    unsigned char *data;

    if(type == BMP || type == TGA)
    {
        data = transform(Image);
        if(data == NULL)
        {
            stringstream foo;
            stringstream bar;
            foo << "createTextureMipMap("<<Image<<", "<<texture<<", "<<type<<")";
            bar << "transform("<<Image<<")";
            setAstral3DError("Can't transform pixel data for BMP/TGA texture", foo.str(), bar.str());
            return false;
            return false;
        }
    }
    else if(type == JPG || type == PNG)
    {
        data = (unsigned char*)Image->pixels;
    }
    else
    {
        stringstream foo;
        stringstream bar;
        foo << "createTextureMipMap("<<Image<<", "<<texture<<", "<<type<<")";
        bar << "N/A";
        setAstral3DError("Unknown texture type (BMP, TGA, JPG and PNG are allowed)", foo.str(), bar.str());
        return false;
    }

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Image->w, Image->h, GL_RGB,
                      GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete data;

    return true;
}

//-----------------------------------------------------------------------------
// uvolni texturu z pameti
//-----------------------------------------------------------------------------

void deleteTexture(GLuint *texture)
{
    glDeleteTextures(1, texture);
}

} // namespace astral3d
