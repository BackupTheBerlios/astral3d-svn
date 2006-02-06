/******************************************************************************

    Astral3D example
    ----------------

    This example shows usage of AConsole class.

    (C) Pavel Stupka 2005

******************************************************************************/

#include <astral3d>

using namespace std;
using namespace astral3d;

//-----------------------------------------------------------------------------
// MyWindow class is a child of AWindow class
//-----------------------------------------------------------------------------

class MyWindow : public AWindow
{
    private:
        AText2D text;      // text for displaying output
        AConsole console;  // console for I/O operations

    public:
        bool init();
        void exit();
        void render();

        // returns pointer to the window console
        AConsole* getConsole() { return &console; }

}window;

//-----------------------------------------------------------------------------
// Console callback function
//-----------------------------------------------------------------------------

void consoleCommand(char *cmd);

//-----------------------------------------------------------------------------
// initialization of the program
//-----------------------------------------------------------------------------

bool MyWindow::init()
{
    // first we create bitmap font from the image
    if(!text.buildFont("font.bmp", 14, 800, 600, 256, 256, 16, 16))
        return false;

    // now we have to set text string describing loaded font
    if(!text.loadString("text.des"))
        return false;

    // creates console from the text above
    if(!console.build(&text, 144, 60, 500, 480, 13))
        return false;

    // loads console background texture
    if(!console.backgroundTexture("console.bmp"))
        return false;

    // sets console font color
    console.setColor(255, 255, 0);

    // associates callback function for the console
    console.setFunc(consoleCommand);

    // associates console with this window
    setConsole(&console);

    // prints some messages to the console
    console.print("This is console example");
    console.print("");
    console.print("using AConsole class");
    console.print("");
    console.print("");
    console.print("press F1 for hide/show");
    console.print("");
    console.print("");
    console.print("type 'exit' for exit");
    console.print("");
    console.print("type 'clear' for clear");
    console.print("");
    console.print("");
    console.print("");

    return true;
}

//-----------------------------------------------------------------------------
// program exit
//-----------------------------------------------------------------------------

void MyWindow::exit()
{
    text.destroyFont();    // frees the memory
    console.destroy();     // destroys the console
}

//-----------------------------------------------------------------------------
// renderes the scene
//-----------------------------------------------------------------------------

void MyWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draws the console
    console.render();
}

//-----------------------------------------------------------------------------
// console callback function
//-----------------------------------------------------------------------------

void consoleCommand(char *cmd)
{
    // if the user writes 'exit' command we close the program
    if(strcmp(cmd, "exit") == 0)
        window.destroy(0);

    // if the user writes 'clear' command we clear the console
    if(strcmp(cmd, "clear") == 0)
        window.getConsole()->clear();
}

//-----------------------------------------------------------------------------
// main function
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    // creates the main window with 800x600 resolution and 16 bpp color depth
    if(!window.create(800, 600, 16))
    {
        // if the create method fails we print error message
        cerr << getAstral3DError();
        return 1;
    }

    window.setCaption("Astral3D - Console Example");
    window.setIcon("icon.bmp");

    // start of the program
    window.run();

    return 0;
}
