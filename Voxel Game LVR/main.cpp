#include "BasicSceneRenderer.h"
#include "Settings.h"

int main()
{
    BasicSceneRenderer app;
	Settings s;
    GLShell::Run(app, "Asteroids 3D", 800, 600);
}
