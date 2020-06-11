#include <iostream>
#include "Game.h"

int main()
{

	Game creategame(600, 600, "game", 120);

	while (!creategame.quit()) {
		creategame.update();
		creategame.render();
	}
}