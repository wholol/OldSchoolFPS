#include "Player.h"

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
