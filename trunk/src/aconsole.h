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
 * @file aconsole.h AConsole class.
 */
#ifndef ACONSOLE_H
#define ACONSOLE_H

#ifdef WIN32
    #include <windows.h>
    #include <SDL.h>
#else
    #include "SDL.h"
#endif

#include <cstdarg>
#include <cstring>
#include <cctype>
#include <string>
#include <sstream>

#include "atext.h"
#include "atexture.h"
#include "aerror.h"
#include "aexceptions.h"

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

/**
 * Class for I/O.
 * This class provides basic interface for communication with user.
 */
class AConsole
{
  private:

      AText2D       *text2D;            // text to display
      int            width;             // console width
      int            height;            // console height
      int            posX, posY;        // left bottom corner
      int            border;            // border

      std::string    commandLine;       // command line text
      std::string   *lineBuffer;        // lines of the console
      int            maxLineChars;      // max chars in the line
      int            noOfLines;         // number of lines except command line

      bool           visible;           // is the console visible?
      SDLKey         consoleOnOffKey;   // button to turn on/off the console

      unsigned char  r, g, b;           // color of the font to display
      bool           fadeOut;           // fade out effect

      bool           userInput;         // is the user input enabled

      unsigned int   texture;           // background texture
      bool           isTextured;        // has the console a texture

      unsigned char  backgroundAlpha;   // alpha color of the background

      void (*func)(char *);             // callback function when 'Enter'
                                        // is pressed

  private:

      void renderBackground();

  public:

      /**
       * Constructor.
       */
      AConsole();
      /**
       * Destructor.
       * Calls AConsole::destroy method.
       * @see destroy
       */
      ~AConsole() { destroy(); }

      /**
       * Constructor.
       * @param text2D AText2D class that will be used for text output
       * @param x x-position of the console on the screen (from left)
       * @param y y-position of the console on the screen (from bottom)
       * @param w Width of the console
       * @param h Height of the console
       * @param border Border width of the console
       * @throw ANullPointerException
       */
      AConsole(AText2D *text2D, int x, int y, int w, int h, int border=0);

      /**
       * Builds the console.
       * This method creates the console.
       * @param text2D AText2D class that will be used for text output
       * @param x x-position of the console on the screen (from left)
       * @param y y-position of the console on the screen (from bottom)
       * @param w Width of the console
       * @param h Height of the console
       * @param border Border width of the console
       * @return Pointer to this instance
       * @throw ANullPointerException
       */
      AConsole *build(AText2D *text2D, int x, int y, int w, int h, int border=0);
      /**
       * Destroys the console.
       * This method destroys the console and frees the memory.
       * Called automatically from the destructor.
       * @see build
       */
      void destroy();

      /**
       * Prints the output to the console.
       * This method prints the output to the console using 'printf' function syntax.
       * @param str Text to write
       */
      void print(const char *str, ...);
      /**
       * Renderes the console.
       * This method renderes the console.
       */
      void render();

      /**
       * Sets the background texture for the console.
       * This method sets the background texture for the console.
       * @param filename Filename of the texture (PNG, TGA, BMP and JPEG formats are supported)
       * @see setBackgroundAlpha
       * @throw ATextureException
       */
      void backgroundTexture(char *filename);

      /**
       * Sets the text color.
       * This method sets the color of the text in the console.
       * @param r Red value of the RGB model (0 to 255)
       * @param g Green value of the RGB model (0 to 255)
       * @param b Blue value of the RGB model (0 to 255)
       */
      inline void setColor(unsigned char r, unsigned char g, unsigned char b);
      /**
       * Sets the transparency.
       * This method sets the transparency of the console background.
       * @param aplha Transparency of the background (0 to 255, 255 is full transparent)
       * @see backgroundTexture
       */
      inline void setBackgroundAlpha(unsigned char alpha);

      /**
       * Sets the console show/hide key.
       * This method sets the key that will be used to show and hide
       * the console. Default show/hide key for the console is F1.
       * @param key SDLKey enumeration type for the keys (see SDL documentation for more info)
       * @see getConsoleOnOffKey
       */
      void   setConsoleOnOffKey(SDLKey key)  { consoleOnOffKey = key; }
      /**
       * Returns the console show/hide key.
       * This method returns the key that is used to show and hide
       * the console. Default show/hide key for the console is F1.
       * @return Key for showing and hiding the console (SDLKey enumertion type)
       * @see getConsoleOnOffKey
       */
      SDLKey getConsoleOnOffKey()            { return consoleOnOffKey; }

      /**
       * Clears the console.
       * This method clears the console.
       */
      void clear();

      /**
       * Sets the callback function.
       * This method sets the callback function. This function is called when 'Enter'
       * key is pressed in the console. Parameter of the callback function is the
       * string containing a text written to the console by user.
       * @param callback Pointer to the function having 'void' return value and '*char' parameter
       * @throw ANullPointerException
       */
      inline void setCallback(void (*callback)(char *));

      /**
       * Shows the console.
       * This method shows the console. You don't have to call this method
       * if the console is associated to the window (AWindow class)
       * using the AWindow::setConsole method. In this case the console is
       * shown and hidden using the key set by the AConsole::setConsoleOnOffKey
       * method.
       * @see setConsoleOnOffKey
       * @see hide
       */
      void show()              { visible=true; }
      /**
       * Hides the console.
       * This method hides the console. You don't have to call this method
       * if the console is associated to the window (AWindow class)
       * using the AWindow::setConsole method. In this case the console is
       * shown and hidden by the key set by the AConsole::setConsoleOnOffKey
       * method.
       * @see setConsoleOnOffKey
       * @see show
       */
      void hide()              { visible=false; }
      /**
       * Enables user input.
       * This method enables user input to the console. By default the user
       * input is enabled.
       * @see disableUserInput
       */
      void enaleUserInput()    { userInput = true; }
      /**
       * Disables user input.
       * This method disables user input to the console. By default the user
       * input is enabled.
       * @see enableUserInput
       */
      void disableUserInput()  { userInput = false; }
      /**
       * Enables fade-out effect.
       * This method enables fade-out effect which means that older console
       * outputs are more transparent than new ones.
       * @see disableFadeOut
       */
      void enableFadeOut()     { fadeOut = true; }
      /**
       * Disables fade-out effect.
       * This method Disables fade-out effect which means that older console
       * outputs aren't more transparent than new ones.
       * @see enableFadeOut
       */
      void disableFadeOut()    { fadeOut = false; }

      /**
       * Returns user input state.
       * This method returns user input state.
       * @return True if the user input is enabled
       */
      bool isUserInput()       { return userInput; }
      /**
       * Returns visibility of the console.
       * @return True if the console is visible
       */
      bool isVisible()         { return visible; }

      /**
       * Writes a character to the console.
       * This method writes a character to the console. This method is called
       * from the AWindow class main loop method if the console is associated
       * to the window (see AWindow::setConsole).
       * @param c Character to write
       * @see keyDownInput
       */
      void input(char c);
      /**
       * User input to the console.
       * This method processes the user input given by the SDL_keysym
       * structure. This method is called from the AWindow class main loop
       * method if the console is associated to the window (see AWindow::setConsole).
       * @param keysym SDL_keysym structure (see SDL documentation for more info)
       * @see input
       */
      void keyDownInput(SDL_keysym *keysym);
};

//-----------------------------------------------------------------------------
//
//    inline functions
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// sets the font color
//-----------------------------------------------------------------------------

void AConsole::setColor(unsigned char r, unsigned char g, unsigned char b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

//-----------------------------------------------------------------------------
// sets the alpha color of the backround
//-----------------------------------------------------------------------------

void AConsole::setBackgroundAlpha(unsigned char alpha)
{
    backgroundAlpha = 255 - alpha;
}

//-----------------------------------------------------------------------------
// sets the callback function
//-----------------------------------------------------------------------------

void AConsole::setCallback(void (*callback)(char *))
{
    if (!callback)
    {
        throw ANullPointerException("void AConsole::setCallback(void (*callback)(char *))");
    }

    func = callback;
}

} // namespace astral3d

#endif    // #ifndef ACONSOLE_H
