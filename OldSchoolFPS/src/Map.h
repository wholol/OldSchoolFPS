#include <string>
#include "Player.h"
#include <SFML\Graphics.hpp>

class Map
{
public:
	Map(int MapWidth, int MapHeight, int ScreenWidth, int ScreenHeight);
	void UpdateMap(Player& player);
	void DrawMap(sf::RenderWindow& window);


private:
	
	int MapWidth;
	int MapHeight;
	int ScreenWidth, ScreenHeight;
	int CeilingStart = 0;
	int FloorStart = 0;
	sf::Image image;
	std::wstring map;
	sf::Texture texture;
	sf::Sprite sprite;

	void ComputePlayerRayCast(PlayerParams& player);
	
	static constexpr float MaxDepth = 16;
	
};