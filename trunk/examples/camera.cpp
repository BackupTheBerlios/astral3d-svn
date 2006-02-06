/******************************************************************************

    Astral3D example
    ----------------

    This example shows usage of ACamera class.

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
        ACamera camera;      // camera for moving in the scene

        bool wKey;           // keys for moving the camera:
        bool sKey;           // w = forward
        bool aKey;           // s = backward
        bool dKey;           // a = step left,  d = step right

        GLuint floor;        // floor texture

    public:
        bool init();
        void exit();
        void loop();
        void render();

        void keyDown(SDL_keysym *keysym);     // keydown event
        void keyUp(SDL_keysym *keysym);       // keyup event

}window;

//-----------------------------------------------------------------------------
// initialization of the program
//-----------------------------------------------------------------------------

bool MyWindow::init()
{
    // not moving
    wKey = sKey = aKey = dKey = false;

    // loads the floor texture
    if(!loadTextureMipMap("floor.bmp", &floor))
        return false;

    camera.enableMouse();     // enables moving camera with the mouse
    camera.setSpeed(0.5);     // sets speed of camera when moving
    camera.setWindow(this);   // associates this window with the camera

    hideCursor();             // and finally hides the mouse cursor

    return true;
}

//-----------------------------------------------------------------------------
// program main loop
//-----------------------------------------------------------------------------

void MyWindow::loop()
{
    // if the given key is down we make a move
    if(wKey) camera.moveForward();
    if(sKey) camera.moveBackward();
    if(aKey) camera.stepLeft();
    if(dKey) camera.stepRight();

    // check the mouse motion of the camera
    camera.mouseProc();
}

//-----------------------------------------------------------------------------
// program exit
//-----------------------------------------------------------------------------

void MyWindow::exit()
{
    // frees the memory
    deleteTexture(&floor);
}

//-----------------------------------------------------------------------------
// renderes the scene
//-----------------------------------------------------------------------------

void MyWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // sets the camera to the scene
    camera.set();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floor);
    glColor3ub(255, 255, 255);

    // draws the floor
    glBegin(GL_QUADS);
        glTexCoord2i( 0,  0);  glVertex3i(-50, -5,  50);
        glTexCoord2i(10,  0);  glVertex3i( 50, -5,  50);
        glTexCoord2i(10, 10);  glVertex3i( 50, -5, -50);
        glTexCoord2i( 0, 10);  glVertex3i(-50, -5, -50);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glLineWidth(10.0);

    // draws coordinate definitions
    glBegin(GL_LINES);
        glColor3ub(255, 0, 0);               // red X axis
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(10.0, 0.0, 0.0);

        glColor3ub(0, 255, 0);               // green Y axis
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);

        glColor3ub(0, 0, 255);               // blue Z axis
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 10.0);
    glEnd();
}

//-----------------------------------------------------------------------------
// keydown window event
//-----------------------------------------------------------------------------

void MyWindow::keyDown(SDL_keysym *keysym)
{
    switch(keysym->sym)
    {
        // end the program with 0 return value
        case SDLK_ESCAPE:
            this->destroy(0);
            break;

        case SDLK_w: wKey = true; break;
        case SDLK_s: sKey = true; break;
        case SDLK_a: aKey = true; break;
        case SDLK_d: dKey = true; break;

        default:
            break;
    }
}

//-----------------------------------------------------------------------------
// keyup window event
//-----------------------------------------------------------------------------

void MyWindow::keyUp(SDL_keysym *keysym)
{
    switch(keysym->sym)
    {
        case SDLK_w: wKey = false; break;
        case SDLK_s: sKey = false; break;
        case SDLK_a: aKey = false; break;
        case SDLK_d: dKey = false; break;

        default:
            break;
    }
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

    window.setCaption("Astral3D - Camera Example");
    window.setIcon("icon.bmp");

    // start of the program
    window.run();

    return 0;
}
