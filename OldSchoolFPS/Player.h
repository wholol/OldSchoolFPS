#pragma once

struct PlayerParams
{
	float player_posx, player_posy;
	float ViewDirection;
	float FOVRad;
	float DistanceToWall;
};


class Player
{
public:
	Player(int ScreenWidth, int ScreenHeight,float FOVDegrees,int player_posx,int player_posy,float ViewDirection);
	PlayerParams GetPlayerParams() const;
	void MoveForward();
	void MoveBackward();
	void RotateLeft();
	void RotateRight();
	

private:
	PlayerParams playerparams;
	static constexpr double Deg2Rad = 3.14159265358979323846f / 180.0f;
	int ScreenHeight, ScreenWidth;

};