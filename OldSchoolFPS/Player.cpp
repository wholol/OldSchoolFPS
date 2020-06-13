#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(int ScreenWidth, int ScreenHeight, float FOVDegrees, int player_posx, int player_posy, float ViewDirection)
	:ScreenWidth(ScreenWidth) , ScreenHeight(ScreenHeight)
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
	playerparams.player_posy += sinf(playerparams.ViewDirection)*0.5f;
	playerparams.player_posx += cosf(playerparams.ViewDirection)*0.5f;
}

void Player::MoveBackward()
{
	playerparams.player_posy -= sinf(playerparams.ViewDirection)*0.5f;
	playerparams.player_posx -= cosf(playerparams.ViewDirection)*0.5f;
}

void Player::RotateLeft()
{
	playerparams.ViewDirection -= 0.05f;
}

void Player::RotateRight()
{
	playerparams.ViewDirection += 0.05f;
}

