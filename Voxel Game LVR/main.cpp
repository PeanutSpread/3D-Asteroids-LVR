#include "GameState.h"
#include "Settings.h"
#include "MenuState.h"

int main() {
    GameState game;
	MenuState menu;
	Settings s;
    //GLShell::Run(menu, "Asteroids 3D", 800, 600);
	GLShell::Run(game, "Asteroids 3D", 800, 600);
}
