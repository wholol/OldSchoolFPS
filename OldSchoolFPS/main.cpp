#include <iostream>

#include "Game.h"

int main()
{

	Game creategame(650, 650, "game", 120);

	while (!creategame.quit()) {
		creategame.update();
		creategame.render();
	}
}