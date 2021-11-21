#include "BasicSceneRenderer.h"
#include "Settings.h"

int main()
{
    BasicSceneRenderer app;
	Settings s;
    GLShell::Run(app, "Asteroids 3D", s.SCREEN_WIDTH, s.SCREEN_HEIGHT);
}
