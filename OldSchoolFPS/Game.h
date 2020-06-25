#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "ImageManager.h"

class Game {			//game class. should have render, update/events, quit and initialize.

public:
	Game(int screenwidth, int screenheight, const std::string& title, int framerate);	//game constructor
	Game &operator=(const Game &other) = delete;				//prevent assigning game object
	Game &operator=(Game &&other) = delete;				//prevent move assigning game object
	Game(const Game& other) = delete;					//no deep copying a game object
	Game(Game &&other) = delete;						//no moving game object
	~Game();

	void main_menu();
	void update();					//update logic
	void render();					//rende r
	bool quit();					//quit the window


private:

	bool quitgame = false;			//quit game
	bool MainMenu = true;

	int FrameCounter = 0;						//frame counter
	Map map;
	Player player;
	Entity entity;

	sf::Event event;					//events class
	sf::RenderWindow window;		//windows class
	bool GameOver = false;				//gameover boolean


};