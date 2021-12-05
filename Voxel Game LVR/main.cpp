#include "GameState.h"
#include "Settings.h"
#include "MenuState.h"

int main() {
    GameState game;
	Settings s;
	GLShell::Run(game, "Asteroids 3D", 800, 600);
}
