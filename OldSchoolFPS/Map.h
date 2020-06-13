#include <string>
#include "Player.h"
#include <SFML\Graphics.hpp>

class Map
{
public:
	Map(int ScreenWidth, int ScreenHeight);
	void UpdateMap(Player& player);
	void DrawMap(sf::RenderWindow& window);
	bool HitWall(Player& player);
private:
	
	int MapWidth = 0;
	int MapHeight = 0;
	int ScreenWidth, ScreenHeight;
	int CeilingStart = 0;
	int FloorStart = 0;
	float SampleWallTextureX = 0.0f;
	float SampleWallTextureY = 0.0f;
	unsigned int wallimagewidth, wallimageheight;
	float MaxDepth;

	sf::Image image;
	std::wstring map;
	
	sf::Texture texture;
	sf::Sprite sprite;
	
	sf::Texture guntexture;
	sf::Sprite gunsprite;

	sf::Image wallimage;

	void ComputePlayerRayCast(PlayerParams& player);
	
	
	
	
};