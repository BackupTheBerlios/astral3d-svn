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
 * @file acamera.h ACamera class.
 */
#ifndef ACAMERA_H
#define ACAMERA_H

#include <iostream>
#include <sstream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef WIN32
    #include <SDL.h>
#else
    #include "SDL.h"
#endif

#include "avector.h"
#include "aabstract.h"
#include "awindow.h"
#include "aerror.h"
#include "aexceptions.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

#define DEFAULT_SPEED 1.0

//-----------------------------------------------------------------------------
// directions
//-----------------------------------------------------------------------------

#define        FORWARD     1
#define        BACKWARD    2
#define        LEFT        4
#define        RIGHT       8
#define        UP          16
#define        DOWN        32

//-----------------------------------------------------------------------------
// ACamera class
//-----------------------------------------------------------------------------

/**
 * Class for moving the camera.
 * This class provides camera movement in the 3D space.
 */
class ACamera : public CollisionEllipsoid
{
  private:
       AVector eye, front, top, right;        // position vectors
       double speed;                          // speed of movement
       double rotX, rotY, rotZ;               // degree of rotation
       double sensitivity;                    // mouse sensitivity
       bool mouse;                            // if the mouse is enabled
       bool collision;                        // if the collision detection is enabled
       AWindow *window;                       // window which this camera is used in

       bool mouseProcedure(int winW, int winH, int mouseX, int mouseY, int *newX, int *newY);

  public:
    /**
     * Constructor..
     * @param vec Camera starting position.
     */
    ACamera(AVector vec = AVector(0.0, 0.0, 0.0));

    /**
     * Constructor.
     * @param window AWindow class to be associated to the camera
     * @throw ANullPointerException
     */
    ACamera(AWindow *window);

    /**
     * Constructor.
     * @param window AWindow class to be associated to the camera
     * @param level Level class to be associated to the camera
     * @throw ANullPointerException
     */
    ACamera(AWindow *window, Level *level);

    /**
     * Sets the scene according to the camera.
     * This method updates the scene view according to the camera view.
     * It should be called before rendering the scene.
     */
    inline   void    set();
    /**
     * Updates camera position.
     * This method updates camera position by adding the vector to the
     * current camera position.
     * @param vec Vector to add
     */
    inline   void    update(AVector vec);
    /**
     * Sets the camera position.
     * This method sets the camera position in the scene.
     * @param vec New camera position
     */
    inline   void    setPosition(AVector vec);
    /**
     * Sets the speed of movement.
     * This method sets the speed of camera movement. Default speed is 1.
     * @param speed New speed of movement.
     * @see getSpeed
     */
    inline   void    setSpeed(double speed);
    /**
     * Returns the speed of movement.
     * This method returns the speed of camera movement.
     * @return Speed of movement of the camera
     * @see setSpeed
     * @see move
     * @see step
     */
    inline   double  getSpeed();

    /**
     * Makes a move according to the given direction.
     * Moves the camera according to the given direction and the speed
     * given by ACamera::setSpeed method.
     *
     * @param dir Direction of movement; allowed directions:<br/>
     * <ul>
     *    <li>FORWARD</li>
     *    <li>BACKWARD</li>
     *    <li>LEFT</li>
     *    <li>RIGHT</li>
     *    <li>UP</li>
     *    <li>DOWN</li>
     * </ul>
     * @see setSpeed
     * @see getSpeed
     * @see step
     * @see moveRight
     * @see moveLeft
     * @see moveForward
     * @see moveBackward
     * @see moveUp
     * @see moveDown
     * @throw AIllegalArgumentException
     */
    void     move(int dir);
    /**
     * Makes a move according to the given direction.
     * Moves the camera according to the given direction and the speed
     * given by ACamera::setSpeed method regardless of camera rotation.
     *
     * @param dir Direction of movement; allowed directions:<br/>
     * <ul>
     *    <li>FORWARD</li>
     *    <li>BACKWARD</li>
     *    <li>LEFT</li>
     *    <li>RIGHT</li>
     *    <li>UP</li>
     *    <li>DOWN</li>
     * </ul>
     * @see setSpeed
     * @see getSpeed
     * @see move
     * @see moveRight
     * @see moveLeft
     * @see moveForward
     * @see moveBackward
     * @see moveUp
     * @see moveDown
     * @throw AIllegalArgumentException
     */
    void     step(int dir);
    /**
     * Rotates the camera.
     * This method rotates the camera according to the given angle.
     *
     * @param dir Direction of the rotation; allowed directions:<br/>
     * <ul>
     *    <li>LEFT</li>
     *    <li>RIGHT</li>
     *    <li>UP</li>
     *    <li>DOWN</li>
     * </ul>
     * @param angle Angle of the rotation in degrees
     * @see turnLeft
     * @see turnRight
     * @see lookUp
     * @see lookDown
     */
    void     turn(int dir, double angle);

    /** Moves the camera.
     * @see move */
    void     moveForward()    { move(FORWARD); }
    /** Moves the camera.
     * @see move */
    void     moveBackward()   { move(BACKWARD); }
    /** Moves the camera.
     * @see move */
    void     moveLeft()       { move(LEFT); }
    /** Moves the camera.
     * @see move */
    void     moveRight()      { move(RIGHT); }
    /** Moves the camera.
     * @see move */
    void     moveUp()         { move(UP); }
    /** Moves the camera.
     * @see move */
    void     moveDown()       { move(DOWN); }

    /** Moves the camera.
     * @see step */
    void     stepForward()    { step(FORWARD); }
    /** Moves the camera.
     * @see step */
    void     stepBackward()   { step(BACKWARD); }
    /** Moves the camera.
     * @see step */
    void     stepLeft()       { step(LEFT); }
    /** Moves the camera.
     * @see step */
    void     stepRight()      { step(RIGHT); }
    /** Moves the camera.
     * @see step */
    void     stepUp()         { step(UP); }
    /** Moves the camera.
     * @see step */
    void     stepDown()       { step(DOWN); }

    /** Rotates the camera.
     * @see rotate */
    void     turnRight(double angle)      { turn(RIGHT, angle); }
    /** Rotates the camera.
     * @see rotate */
    void     turnLeft(double angle)       { turn(LEFT, angle); }
    /** Rotates the camera.
     * @see rotate */
    void     lookUp(double angle)         { turn(UP, angle); }
    /** Rotates the camera.
     * @see rotate */
    void     lookDown(double angle)       { turn(DOWN, angle); }

    /**
     * Enables collision detection.
     * This method enables collision detection.
     * @see disableCollision
     * @see setLevel
     */
    void     enableCollision() { this->collision = true; }
    /**
     * Disables collision detection.
     * This method disables collision detection.
     * @see enabaleCollision
     * @see setLevel
     */
    void     disableCollision() { this->collision = false; }
    /**
     * Moves the camera according to the gravity of the level.
     * This method moves the camera according to the gravity of the level.
     * @see setLevel
     * @see enableCollision
     * @see disableCollision
     */
    void     checkLevelGravity();

    /**
     * Enables mouse control of the camera.
     * This method enables mouse control of the camera.
     * @see disableMouse
     * @see setMouseSensitivity
     * @see mouseProcedure
     */
    void     enableMouse()    { mouse = true; }
    /**
     * Disables mouse control of the camera.
     * This method disables mouse control of the camera.
     * @see enableMouse
     * @see setMouseSensitivity
     * @see mouseProcedure
     */
    void     disableMouse()   { mouse = false; }
    /**
     * Sets the mouse sensitivity.
     * This method sets the mouse sensitivity.
     * @param sens Mouse sensitivity (default is 10)
     * @see enableMouse
     * @see disableMouse
     * @see mouseProcedure
     * @see getMouseSensitivity
     */
    void     setMouseSensitivity(double sens) { sensitivity = sens; }
    /**
     * Returns mouse sensitivity.
     * This method returns mouse sensitivity.
     * @return Mouse sensitivity
     * @see enableMouse
     * @see disableMouse
     * @see mouseProcedure
     * @see setMouseSensitivity
     */
    double   getMouseSensitivity() { return sensitivity; }

    /**
     * Main mouse procedure.
     * This method should be called in the main loop of the program.
     * It sets new camera position according to the mouse movement.
     * @see setWindow
     * @see enableMouse
     * @see disableMouse
     * @see setMouseSensitivity
     */
    void     mouseProcedure();

    /**
     * Sets the window to the camera.
     * This method sets the window to the camera. This method should be called
     * before calling ACamera::mouseProcedure.
     * @param window Window to be set
     * @see mouseProcedure
     * @see enableMouse
     * @see disableMouse
     * @throw ANullPointerException
     */
    void     setWindow(AWindow *window)
    {
        if (!window)
        {
            throw ANullPointerException("void ACamera::setWidnow(AWindow *window)");
        }
        this->window = window;
    }

    /**
     * Returns the position of the camera.
     * @return Position of the camera
     */
    AVector  getEye() { return this->eye; }
    /**
     * Returns the front point of the camera.
     * @return Front point of the camera
     */
    AVector  getFront() { return this->front; }
    /**
     * Returns the top point of the camera.
     * @return Top point of the camera
     */
    AVector  getTop() { return this->top; }
    /**
     * Returns the right point of the camera.
     * @return Right point of the camera
     */
    AVector  getRight() { return this->right; }
};


//-----------------------------------------------------------------------------
//
//    INLINE FUNCTIONS
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// sets the scene according the camera
//-----------------------------------------------------------------------------

void ACamera::set()
{
    gluLookAt(this->eye.x, this->eye.y, this->eye.z,
        this->front.x, this->front.y, this->front.z,
        0.0, 1.0, 0.0);
}

//-----------------------------------------------------------------------------
// adds the given vector to the camera position
//-----------------------------------------------------------------------------

void ACamera::update(AVector a)
{
    this->eye += a;
    this->front += a;
    this->top += a;
    this->right += a;
}

//-----------------------------------------------------------------------------
// sets the camera position
//-----------------------------------------------------------------------------

void ACamera::setPosition(AVector a)
{
    this->eye = this->front = this->top = this->right = a;

    this->rotX = this->rotY = this->rotZ = 0.0;

    this->front.z -= 1.0;
    this->top.y += 1.0;
    this->right.x += 1.0;
}

//-----------------------------------------------------------------------------
// sets the speed movement
//-----------------------------------------------------------------------------

void ACamera::setSpeed(double s)
{
    this->speed = s;
}

//-----------------------------------------------------------------------------
// return the speed movement
//-----------------------------------------------------------------------------

double ACamera::getSpeed()
{
    return speed;
}

} // namespace astral3d

#endif    // #ifndef ACAMERA_H
