#include <string>
#include "Player.h"
#include <SFML\Graphics.hpp>
#include "Enemy.h"

class Map
{
public:
	Map(int ScreenWidth, int ScreenHeight);
	void UpdateMap(Player& player, Enemy& enemy);
	void DrawMap(sf::RenderWindow& window);
	bool HitWall(Player& player);
	std::wstring GetMap() const;
	int GetMapWidth() const;
	int GetMapHeight() const;

private:
	
	int MapWidth = 0;
	int MapHeight = 0;
	int ScreenWidth, ScreenHeight;
	int CeilingStart = 0;
	int FloorStart = 0;
	float SampleWallTextureX = 0.0f;
	float SampleWallTextureY = 0.0f;
	float SampleEnemyTextureX = 0.0f;
	float SampleEnemyTextureY = 0.0f;
	std::vector<float> ZBuffer;
	unsigned int wallimagewidth, wallimageheight;
	float MaxDepth;

	sf::Image image;
	std::wstring map;
	
	sf::Texture texture;
	sf::Sprite sprite;
	
	sf::Texture guntexture;
	sf::Sprite gunsprite;

	sf::Image wallimage;

	void DrawBackGround(PlayerParams& p);
	void DrawEnemies(std::vector<EnemyParams>& e, PlayerParams& p);
};