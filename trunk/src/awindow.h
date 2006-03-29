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
 * @file awindow.h AWindow class.
 */

#ifndef AWINDOW_H
#define AWINDOW_H

#ifdef WIN32
    #include <windows.h>
    #include <SDL.h>
#else
    #include "SDL.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cstdlib>
#include <sstream>

#include "aconsole.h"
#include "aerror.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

/**
 * Exit function.
 * @param exitCode Exit code of the program
 */
void appExit(int exitCode);

/**
 * Class for manipulating windows.
 * This class is the interface into the application. It can handle system events
 * such as pressing the key or moving the mouse. Typical use of the AWindow
 * class is to create your own class inheritting from AWindow class and
 * override some important methods. These methods are virtual in AWindow class.
 * @n
 * @n
 * Your new window class should look like this:
 * @n
 * @code
 * class MyWindow : public AWindow
 * {
 * public:
 *   bool init();
 *   void loop();
 *   void exit();
 *   void render();
 *   void keyDown(SDL_keysym *keysym);
 *   void keyUp(SDL_keysym *keysym);
 * };
 * @endcode
 */
class AWindow
{
protected:

    SDL_Surface *screen;    // main window screen
    bool fullscreen;        // fullscreen mode indication
    int width;              // window width
    int height;             // window height
    int bpp;                // bit depth
    int flags;              // video informations
    int mouseX;             // x cursor position
    int mouseY;             // y cursor position
    AConsole *console;      // console for keyDown callback

    void initGL();          // inicilizes OpenGL interface
    bool running;

public:

    /**
     * Constructor.
     */
    AWindow() {};
    /**
     * Destructor.
     */
    virtual ~AWindow() { destroy(0); }

    /**
     * Constructor.
     * @param width Window width
     * @param height Window height
     * @param bpp Color depth
     * @param resizable Resizable window
     * @param fullscreen Fullscreen mode
     * @throw ASDLException
     * @throw AException
     */
    AWindow(int width, int height, int bpp, bool resizable = true, bool fullscreen = false);

    /**
     * Sets the viewport after the window resize event.
     * This method sets the viewport after the window resize event and do
     * necessary OpenGL settings. It is called automatically from the main
     * program loop (see AWindow::run) after the resize event is catched.
     * By default it sets perspective viewport. Override this method to set
     * your own viewport.
     * @param width New window width
     * @param height New window height
     */
    virtual void resizeScreen(int width, int height);
    /**
     * Renderes the scene.
     * This method should be overriden. It is called automatically from the
     * main loop. This method should contain code for rendering the scene.
     * @see run
     * @see loop
     */
    virtual void render();
    /**
     * Keydown callback method.
     * This method is called automatically from the main loop after the
     * keydown event is catched. This method should be overriden.
     * @n
     * @n
     * Example of usage:
     * @n
     * @code
     * void MyWindow::keyDown(SDL_keysym *keysym)
     * {
     *     switch(keysym->sym)
     *     {
     *         case SDLK_ESCAPE:
     *              this->destroy(0);
     *              break;
     *
     *         case SDLK_w:
     *              w = true;
     *              break;
     *
     *         default:
     *              break;
     *     }
     * }
     * @endcode
     * @param keysym SDL_keysym struct containing info about the keys
     * @see keyUp
     */
    virtual void keyDown(SDL_keysym *keysym) {}
    /**
     * Keyup callback method.
     * This method is called automatically from the main loop after the
     * keyup event is catched. This method should be overriden.
     * @n
     * @n
     * Example of usage:
     * @n
     * @code
     * void MyWindow::keyUp(SDL_keysym *keysym)
     * {
     *     switch(keysym->sym)
     *     {
     *         case SDLK_w:
     *              w = false;
     *              break;
     *
     *         default:
     *              break;
     *     }
     * }
     * @endcode
     * @param keysym SDL_keysym struct containing info about the keys
     * @see keyDown
     */
    virtual void keyUp(SDL_keysym *keysym) {}
    /**
     * Window loop.
     * This method is the main loop called before AWindow::render method.
     * This method should be overriden. It is called automatically from
     * AWindow::run
     * @see render
     * @see run
     */
    virtual void loop() {}
    /**
     * Application init method.
     * This method inicializes the application.
     * This method should be overriden. It is called automatically
     * from AWindow::create when building the window.
     * @return True if the initialization is OK
     * @see create
     * @see exit
     */
    virtual bool init() { return true; }
    /**
     * Application exit method.
     * This method is called automatically when destroying the window.
     * This method should be overriden.
     * @see init
     * @see destroy
     */
    virtual void exit() {}
    /**
     * Mousedown callback method.
     * This method is called automatically from the main loop after the
     * mousedown event is catched. This method should be overriden.
     * @param mouseX x-mouse position
     * @param mouseY x-mouse position
     * @param button Pressed button
     */
    virtual void mouseButtonDown(int mouseX, int mouseY, int button) {}
    /**
     * Mouseup callback method.
     * This method is called automatically from the main loop after the
     * mouseup event is catched. This method should be overriden.
     * @param mouseX x-mouse position
     * @param mouseY y-mouse position
     * @param button Releassed button
     */
    virtual void mouseButtonUp(int mouseX, int mouseY, int button) {}

    /**
     * Creates the window.
     * This method creates the window and inicializes it. It calls
     * AWindow::init method at the end.
     * @param width Window width
     * @param height Window height
     * @param bpp Color depth
     * @param resizable Resizable window
     * @param fullscreen Fullscreen mode
     * @return Pointer to this instance
     * @throw ASDLException
     * @throw AException
     * @see init
     */
    AWindow *create(int width, int height, int bpp, bool resizable = true, bool fullscreen = false);
    /**
     * Destroys the window.
     * This method destroys the window and frees the memory. It calls
     * AWindow::exit method.
     * @param retVal Return value of the program
     * @see exit
     */
    void destroy(int retVal);
    /**
     * Main window loop.
     * This is the main window loop. You have to call AWindow::create
     * method before entering the main loop. When you call this method SDL
     * events are processed and corresponding callback methods
     * (such as AWindow::keyDown) are automatically called.
     * @see create
     */
    void run();
    /**
     * Changes the fullscreen mode.
     * This method changes the fullscreen mode.
     * @param fullscreen Fullscreen mode
     */
    void toggleFullscreen(bool fullscreen);
    /**
     * Sets the window caption.
     * This method sets the window caption.
     * @param caption Window caption
     * @see setIcon
     */
    void setCaption(char *caption);
    /**
     * Sets the window icon.
     * This method sets the window icon. In MS Windows this icon must be
     * 32x32 pixels.
     * @param caption Window icon
     * @see setCaption
     */
    void setIcon(char *filename);

    /**
     * Returns window width.
     * This method returns window width.
     * @return Window width
     */
    int  getWidth()      { return this->width; }
    /**
     * Returns window height.
     * This method returns window height.
     * @return Window height
     */
    int  getHeight()     { return this->height; }
    /**
     * Returns window depth.
     * This method return window depth.
     * @return Bits per pixel of the window
     */
    int  getBpp()        { return this->bpp; }
    /**
     * Returns fullscreen mode.
     * This method returns 'true' if the window is in fullscreen mode.
     * @return 'true' if the window is in fullscreen mode
     */
    bool getFullscreen() { return this->fullscreen; }
    /**
     * Returns X cursor position.
     * This method returns X cursor position.
     * @return X cursor position
     */
    int  getMouseX()     { return this->mouseX; }
    /**
     * Returns Y cursor position.
     * This method returns Y cursor position.
     * @return Y cursor position
     */
    int  getMouseY()     { return this->mouseY; }
    /**
     * Returns window parameters.
     * This method return all window parameters.
     * @n
     * @n
     * Example of usage
     * @n
     * @code
     * int flags = window->getFlags()
     * if((flags & SDL_RESIZABLE) == SDL_RESIZABLE)
     * {
     *     // window is resizable
     * }
     * @endcode
     *
     * @return Window parameters
     */
    int  getFlags()      { return this->flags; }

    /**
     * Hides the cursor.
     * This method hides the cursor.
     * @see showCursor
     */
    void hideCursor()    { SDL_ShowCursor(SDL_DISABLE); }
    /**
     * Shows the cursor.
     * This method shows the cursor.
     * @see hideCursor
     */
    void showCursor()    { SDL_ShowCursor(SDL_ENABLE); }
    /**
     * Sets the console for keyDown callback.
     * This method associates console to the window. KeyDown events are
     * forwarded to this console and user can do text input to the console.
     * @param console Console to be set
     * @throw ANullPointerException
     */
    void setConsole(AConsole *console)
    {
        if (!console)
        {
            throw ANullPointerException("void setConsole(AConsole *console) -> console is NULL");
        }
        this->console = console;
    }
};

} // namespace astral3d

#endif // #ifndef AWINDOW_H
