/******************************************************************************

    Astral3D example
    ----------------

    This example shows usage of AText2D class.

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

    public:
        bool init();
        void exit();
        void render();

}window;

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

    return true;
}

//-----------------------------------------------------------------------------
// program exit
//-----------------------------------------------------------------------------

void MyWindow::exit()
{
    text.destroyFont();    // frees the memory
}

//-----------------------------------------------------------------------------
// renderes the scene
//-----------------------------------------------------------------------------

void MyWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3ub(255, 255, 0);
    text.setSize(16, 16, 14);
    text.print(100, 400, "This is text output example");

    glColor3ub(255, 0, 0);
    text.setSize(16, 32, 15);
    text.print(400, 250, "using AText2D class");

    glColor3ub(0, 255, 0);
    text.setSize(80, 32, 65);
    text.print(50, 100, "Astral3D");
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

    window.setCaption("Astral3D - Text Example");
    window.setIcon("icon.bmp");

    // start of the program
    window.run();

    return 0;
}
