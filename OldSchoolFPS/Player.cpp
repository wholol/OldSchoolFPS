#include "Player.h"
#include "Map.h"
#include <cmath>
#include <iostream>

Player::Player(int ScreenWidth, int ScreenHeight, float FOVDegrees, int player_posx, int player_posy, float ViewDirection , const Map& map)
	:ScreenWidth(ScreenWidth) , ScreenHeight(ScreenHeight),map(map)
{
	playerparams.player_posx = player_posx;
	playerparams.player_posy = player_posy;
	playerparams.FOVRad = FOVDegrees * Deg2Rad;
	playerparams.ViewDirection = ViewDirection;
	playerparams.DistanceToWall = 0.0f;
}

PlayerParams Player::GetPlayerParams() const
{
	return playerparams;
}

void Player::MoveForward()
{
	float next_posy = playerparams.player_posy += sinf(playerparams.ViewDirection)*0.5f;
	float next_posx = playerparams.player_posx += cosf(playerparams.ViewDirection)*0.5f;

	if (map.GetMap()[(int)next_posx + map.GetMapWidth() * (int)next_posy] == '#')
	{
		playerparams.player_posy -= sinf(playerparams.ViewDirection)*0.5f;
		playerparams.player_posx -= cosf(playerparams.ViewDirection)*0.5f;
	}

}

void Player::MoveBackward()
{
	float next_posy = playerparams.player_posy -= sinf(playerparams.ViewDirection)*0.5f;
	float next_posx = playerparams.player_posx -= cosf(playerparams.ViewDirection)*0.5f;

	if (map.GetMap()[(int)next_posx + map.GetMapWidth() * (int)next_posy] == '#')
	{
		playerparams.player_posy += sinf(playerparams.ViewDirection)*0.5f;
		playerparams.player_posx += cosf(playerparams.ViewDirection)*0.5f;
	}
}

void Player::RotateLeft()
{
	playerparams.ViewDirection -= 0.05f;
}

void Player::RotateRight()
{
	playerparams.ViewDirection += 0.05f;
}



