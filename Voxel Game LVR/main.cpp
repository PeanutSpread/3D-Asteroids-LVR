#include "BasicSceneRenderer.h"
#include "Settings.h"

int main()
{
    BasicSceneRenderer app;
	Settings s;
    GLShell::Run(app, "Basic Scene Renderer", s.SCREEN_WIDTH, s.SCREEN_HEIGHT);
}
