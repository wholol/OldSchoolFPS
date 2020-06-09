#pragma once


struct PlayerParams
{
	int player_posx, player_posy;
	int ViewDirection;
	float FOVRad;
	float DistanceToWall;
};


class Player
{
public:
	Player(int ScreenWidth, int ScreenHeight,float FOVDegrees,int player_posx,int player_posy,float ViewDirection);
	PlayerParams GetPlayerParams() const;
	

private:
	PlayerParams playerparams;
	static constexpr float Deg2Rad = 3.14159 / 180;
	int ScreenHeight, ScreenWidth;
};