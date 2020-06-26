#include "Game.h"
#include <iostream>
#include <thread>

Game::Game(int screenwidth, int screenheight, const std::string& title, int framerate)
	:window(sf::VideoMode(screenwidth, screenheight), title)
	,map(screenwidth,screenheight)
	,player(screenwidth, screenheight, 45,  8,7, 0,map),
	entity(map)
{
	/*load images that will be used*/
	ImageManager::GetInstanace().addImage("gun.png");
	ImageManager::GetInstanace().addImage("bricks.png");
	ImageManager::GetInstanace().addImage("enemy.png");
	ImageManager::GetInstanace().addImage("bullet.png");
	
	/*initialzie images used for the map*/
	map.InitImages();

	/*initialize entities here*/
	entity.addEntity(4, 4 ,true, "enemy.png" , 100 );
	entity.addEntity(20, 23 ,true, "enemy.png", 100);
	entity.addEntity(14, 14, true, "enemy.png", 100);
	entity.addEntity(14, 24, true, "enemy.png", 100);
}

void Game::render() {		//rendering
	map.DrawMap(window);
	window.display();
}

void Game::main_menu()
{
}

void Game::update() {		//update game 
	std::this_thread::sleep_for(std::chrono::milliseconds(30));

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
		if (!map.CollideWall(player)) {
			player.MoveForward();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (!map.CollideWall(player)) {
			player.MoveBackward();
		}
	}

	//shoot
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		auto playerparams = player.GetPlayerParams();
  		entity.addEntity(playerparams.player_posx , playerparams.player_posy, false, "bullet.png");
		std::this_thread::sleep_for(std::chrono::milliseconds(35));
		entity.MoveEntities(player);
	}

	entity.MoveEntities(player);//move movable entites (bullet)
	entity.DeleteEntity();		//delete living entities whos health fall below zero.

	map.UpdateMap(player,entity);
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
