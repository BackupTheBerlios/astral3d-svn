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

#include "acamera.h"

using namespace std;
namespace astral3d {

// upravi uhel
void update_angle(double *uhel)
{
   double kruh = 360.0f;

   if(*uhel >= kruh){
      int   m = (int) (*uhel / kruh);
      *uhel = *uhel - m * kruh;}
   if(*uhel < 0){
      int   m = (int) (-1*(*uhel) / kruh + 1);
      *uhel = m * kruh - *uhel*-1;}
}

//----------------------------------------------------------------------
// konstruktor
//----------------------------------------------------------------------

ACamera::ACamera(AVector a)
{
    this->eye = this->front = this->top = this->right = a;

    this->mouse = false;
    this->sensitivity = 10.0;

    this->collision = false;

    this->rotX = this->rotY = this->rotZ = 0.0;

    this->front.z -= 1.0;
    this->top.y += 1.0;
    this->right.x += 1.0;

    this->speed = DEFAULT_SPEED;
}

//----------------------------------------------------------------------
// procedura mysi
//----------------------------------------------------------------------

void ACamera::mouseProc()
{
    if(window)
    {
        int x, y;
        if(mouseProcedure(window->getWidth(), window->getHeight(),
                          window->getMouseX(), window->getMouseY(), &x, &y))
            SDL_WarpMouse(x, y);
    }
}

//----------------------------------------------------------------------
// pohyb podle smeru
//----------------------------------------------------------------------

void ACamera::move(int direction)
{
    AVector v(0.0, 0.0, 0.0);

    if((direction & FORWARD) == FORWARD)
        v = v + (this->front - this->eye);

    if((direction & BACKWARD) == BACKWARD)
        v = v + (this->eye - this->front);

    if((direction & LEFT) == LEFT)
        v = v + (this->eye - this->right);

    if((direction & RIGHT) == RIGHT)
        v = v + (this->right - this->eye);

    if((direction & UP) == UP)
        v = v + (this->top - this->eye);

    if((direction & DOWN) == DOWN)
        v = v + (this->eye - this->top);

    // udela pohyb
    v.normalize();
    v *= this->speed;

    if(this->collision)
    {
        AVector newVel = this->level->getDirection(this->eye, v);
        v = newVel;
    }

    this->update(v);
}

//----------------------------------------------------------------------
//    special kind of move
//----------------------------------------------------------------------

void ACamera::step(int direction)
{
    AVector v(0.0, 0.0, 0.0);

    if((direction & FORWARD) == FORWARD)
    {
        v = v + (this->front - this->eye);
        v.y = 0.0;
    }

    if((direction & BACKWARD) == BACKWARD)
    {
        v = v + (this->eye - this->front);
        v.y = 0.0;
    }

    if((direction & LEFT) == LEFT)
    {
        v = v + (this->eye - this->right);
        v.y = 0.0;
    }

    if((direction & RIGHT) == RIGHT)
    {
        v = v + (this->right - this->eye);
        v.y = 0.0;
    }

    if((direction & UP) == UP)
    {
        v.x = 0.0;
        v.y = 1.0;
        v.z = 0.0;
    }

    if((direction & DOWN) == DOWN)
    {
        v.x = 0.0;
        v.y = -1.0;
        v.z = 0.0;
    }

    // udela pohyb
    v.normalize();
    v *= this->speed;

    if(this->collision)
    {
        AVector newVel = this->level->getDirection(this->eye, v);
        v = newVel;
    }

    this->update(v);
}

//----------------------------------------------------------------------
// provadi pohyb v zavislosti na gravitaci levelu, melo by byt volano
// stale :)
//----------------------------------------------------------------------

void ACamera::checkLevelGravity()
{
    if(level != NULL && level->isGravityEnabled())
    {
        AVector v = this->level->getGravityDirection(this->eye);
        this->update(v);
    }
}

//----------------------------------------------------------------------
// procedura mysi
//----------------------------------------------------------------------

bool ACamera::mouseProcedure(int w, int h, int mouseX, int mouseY,
                             int *new_x, int *new_y)
{
    if(!this->mouse) return false;

    int x = mouseX - w/2;
    int y = mouseY - h/2;

    this->turnRight(x/sensitivity);
    this->lookDown(y/sensitivity);

    if((this->rotX > 80) && (this->rotX < 180)) lookUp(y/this->sensitivity);
    if((this->rotX < 280) && (this->rotX > 180)) lookUp(y/this->sensitivity);

    *new_x = w/2;
    *new_y = h/2;

    return true;
}

//----------------------------------------------------------------------
// otoceni kamery
//----------------------------------------------------------------------

void ACamera::turn(int direction, double angle)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    AVector v;

    switch(direction)
    {
        case LEFT:
            this->rotY -= angle;
            update_angle(&this->rotY);
            glRotated(angle, 0.0, 1.0, 0.0);
            break;

        case RIGHT:
            this->rotY += angle;
            update_angle(&this->rotY);
            glRotated(-angle, 0.0, 1.0, 0.0);
            break;

        case UP:
            this->rotX -= angle;
            update_angle(&this->rotX);
            v = this->right - this->eye;
            glRotated(angle, v.x, v.y, v.z);
            break;

        case DOWN:
            this->rotX += angle;
            update_angle(&this->rotX);
            v = this->right - this->eye;
            glRotated(-angle, v.x, v.y, v.z);
            break;
    }

    double data[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, data);

    this->front -= this->eye;
    this->top -= this->eye;
    this->right -= this->eye;

    this->right.applyMatrix(data);
    this->front.applyMatrix(data);
    this->top.applyMatrix(data);

    this->front += this->eye;
    this->top += this->eye;
    this->right += this->eye;

    glPopMatrix();
}

} // namespace astral3d
