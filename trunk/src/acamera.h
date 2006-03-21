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
#include "alevel.h"
#include "awindow.h"
#include "aerror.h"

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
 * Class for moving the camera in the 3D space.
 * This class provides camera movement in the 3D space.
 * ACamera is also designed to do collision detection against
 * the level represented by the ALevel class.
 */
class ACamera
{
  private:
       AVector eye, front, top, right;        // position vectors
       double speed;                          // speed of movement
       double rotX, rotY, rotZ;               // degree of rotation
       double sensitivity;                    // mouse sensitivity
       bool mouse;                            // if the mouse is enabled
       bool collision;                        // if the collision detection is enabled
       ALevel *level;                         // level for collision detection
       AWindow *window;                       // window which this camera is used in

       bool mouseProcedure(int winW, int winH, int mouseX, int mouseY, int *newX, int *newY);

  public:
    /**
     * Constructor.
     * Constructor sets the camera position to the start position.
     * Default start position is AVector(0.0, 0.0, 0.0).
     * @param vec Camera start position
     */
    ACamera(AVector vec = AVector(0.0, 0.0, 0.0));

    /**
     * Sets the camera to the scene.
     * This method updates the scene view according to the camera view.
     * It should be called before rendering the scene.
     */
    inline   void    set();
    /**
     * Updates camera position.
     * This method updates camera position by adding the given vector to the
     * current camera position.
     * @param vec Vector (AVector) to add
     */
    inline   void    update(AVector vec);
    /**
     * Sets the camera position (deprecated).
     * This method sets the camera position in the scene.
     * @param vec New camera position (AVector)
     * @see setPosition
     */
    inline   void    setPos(AVector vec);
    /**
     * Sets the camera position.
     * This method sets the camera position in the scene.
     * @param vec New camera position (AVector)
     */
    inline   void    setPosition(AVector vec);
    /**
     * Sets the speed of mevement.
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
     */
    inline   double  getSpeed();

    /**
     * Makes a move according to the given direction.
     * Moves the camera according to the given direction and the speed
     * given by ACamera::setSpeed method.
     *
     * @param dir Direction, allowed directions: FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
     * @see setSpeed
     * @see getSpeed
     * @see step
     * @see moveRight
     * @see moveLeft
     * @see moveForward
     * @see moveBackward
     * @see moveUp
     * @see moveDown
     */
    void     move(int dir);
    /**
     * Makes a move according to the given direction.
     * Moves the camera according to the given direction and the speed
     * given by ACamera::setSpeed method.
     *
     * @param dir Direction, allowed directions: FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
     * @see setSpeed
     * @see getSpeed
     * @see move
     * @see stepRight
     * @see stepLeft
     * @see stepForward
     * @see stepBackward
     * @see stepUp
     * @see stepDown
     */
    void     step(int dir);
    /**
     * Rotation.
     * This method rotates the camera according to the given angle.
     * @param dir Direction of turning the camera, allowed directions: LEFT, RIGHT, UP, DOWN
     * @param angle Angle in degrees
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
     * Sets the level for collision detections.
     * This method sets the camera level (ALevel) for collision detections.
     * @param *level Level (ALevel) for collison detections
     * @see enableCollision
     * @see disableCollision
     */
    void     setLevel(ALevel *level) { this->level = level; }
    /**
     * Moves the camera according to the gravitation of the level.
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
     * @see mouseProc
     */
    void     enableMouse()    { mouse = true; }
    /**
     * Disables mouse control of the camera.
     * This method disables mouse control of the camera.
     * @see enableMouse
     * @see setMouseSensitivity
     * @see mouseProc
     */
    void     disableMouse()   { mouse = false; }
    /**
     * Sets the mouse sensitivity.
     * This method sets the mouse sensitivity.
     * @param sens Mouse sensitivity (default is 10)
     * @see enableMouse
     * @see disableMouse
     * @see mouseProc
     */
    void     setMouseSensitivity(double sens) { sensitivity = sens; }
    /**
     * Returns mouse sensitivity.
     * This method returns mouse sensitivity.
     * @return Mouse sensitivity
     * @see enableMouse
     * @see disableMouse
     * @see mouseProc
     */
    double   getMouseSensitivity() { return sensitivity; }

    /**
     * Main mouse procedure (deprecated).
     * This method should be called in the main loop of the program.
     * It sets new camera position according to the mouse movement.
     * @see setWindow
     * @see enableMouse
     */
    void     mouseProc();

    /**
     * Main mouse procedure.
     * This method should be called in the main loop of the program.
     * It sets new camera position according to the mouse movement.
     * @see setWindow
     * @see enableMouse
     */
    void     mouseProcedure() { this->mouseProc(); }

    /**
     * Associates the window with the camera.
     * This method sets the windows to the camera. You should call this method
     * before calling ACamera::mouseProcedure.
     * @param win Window (AWindow) to accosiate
     * @see mouseProc
     * @see enableMouse
     */
    void     setWindow(AWindow *win) { this->window = win; }

    AVector  getEye() { return this->eye; }
    AVector  getFront() { return this->front; }
    AVector  getTop() { return this->top; }
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

void ACamera::setPos(AVector a)
{
    this->eye = this->front = this->top = this->right = a;

    this->rotX = this->rotY = this->rotZ = 0.0;

    this->front.z -= 1.0;
    this->top.y += 1.0;
    this->right.x += 1.0;
}

//-----------------------------------------------------------------------------
// sets the camera position
//-----------------------------------------------------------------------------

void ACamera::setPosition(AVector a)
{
    this->setPos(a);
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
