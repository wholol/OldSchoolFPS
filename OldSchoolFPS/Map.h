#include <string>
#include "Player.h"
#include <SFML\Graphics.hpp>
#include "Enemy.h"
#include "ImageManager.h"

class Map
{
public:

	Map(int ScreenWidth, int ScreenHeight);
	void InitImages();				
	void UpdateMap(Player& player, Entity& enemy);
	void DrawMap(sf::RenderWindow& window);
	bool CollideWall(Player& player);
	std::wstring GetMap() const;	
	int GetMapWidth() const;
	int GetMapHeight() const;

private:
	
	int MapWidth = 0;
	int MapHeight = 0;
	int ScreenWidth, ScreenHeight;
	int CeilingStart = 0;
	int FloorStart = 0;
	std::wstring map;

	/*sampling coordinates*/
	float SampleWallTextureX = 0.0f;
	float SampleWallTextureY = 0.0f;
	float SampleEnemyTextureX = 0.0f;
	float SampleEnemyTextureY = 0.0f;
	
	std::vector<float> ZBuffer;

	/*wall params*/
	unsigned int wallimagewidth, wallimageheight;
	float MaxDepth;
	sf::Image wallimage;	//image for wall texture (image used since pixel positon is needed.)


	/*screen params*/
	sf::Image ScreenImage;		
	sf::Texture ScreenTexture;
	sf::Sprite ScreenSprite;

	/*gun params*/
	sf::Texture guntexture;
	sf::Sprite gunsprite;

	/*helper methods*/
	void DrawBackGround(PlayerParams& p);
	void DrawEntities(std::vector<std::shared_ptr<EntityParams>>& e, PlayerParams& p);
	void LoadMapFromFile();
};