#include "Game.h"

Game::Game(int screenwidth, int screenheight, const std::string& title, int framerate)
	:window(sf::VideoMode(screenwidth, screenheight), title)
	,map(screenwidth,screenheight)
	,player(screenwidth, screenheight, 45,  8,7, 0)

{

}


void Game::render() {		//rendering
	map.DrawMap(window);
	window.display();
}

void Game::main_menu()
{

}

void Game::update() {		//update game 

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		player.RotateRight();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		player.RotateLeft();
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (!map.HitWall(player)) {
			player.MoveForward();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (!map.HitWall(player)) {
			player.MoveBackward();
		}
	}


	map.UpdateMap(player);
	
	window.clear();

}


bool Game::quit()			//call quit game

//DO NOT DELETE CONTENTS OF THIS FUNCTION TO PREVENT AN UNRESPONSIV SCREEN//
{
	while (window.pollEvent(event)) {		//NON BLOCKING FUNCTION, WILL NOT GET STUCK IN WHILE LOOP.
		if (event.type == sf::Event::Closed) {
			window.close();
			quitgame = true;
		}
	}
	return quitgame;

}

Game::~Game() {}
