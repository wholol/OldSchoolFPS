#include "Map.h"
#include <fstream>
#include <algorithm>

Map::Map(int ScreenWidth, int ScreenHeight)
	:ScreenWidth(ScreenWidth), ScreenHeight(ScreenHeight)
{
	for (int i = 0; i < ScreenWidth; ++i)
	{
		ZBuffer.push_back(INFINITY);
	}

	LoadMapFromFile();		//Load map
}

void Map::InitImages()
{
	/*NON-ENTITY IMAGES ARE LOADED HERE.*/
	//load wall image
	wallimage = ImageManager::GetInstanace().getImage("bricks.png");
	wallimagewidth = wallimage.getSize().x;
	wallimageheight = wallimage.getSize().y;
	
	//load gun image
	auto& gunImage = ImageManager::GetInstanace().getImage("gun.png");
	guntexture.loadFromImage(gunImage);
	gunsprite.setTexture(guntexture);
	gunsprite.setPosition(sf::Vector2f((ScreenWidth - gunImage.getSize().x) / 2, (ScreenHeight - gunImage.getSize().y)));
}

void Map::UpdateMap(Player& player, Entity& entity)
{
	PlayerParams p = player.GetPlayerParams();		//get palyer parameters
	std::vector<std::shared_ptr<EntityParams>> e = entity.GetEntity();	//get enemy parameters
	DrawBackGround(p);
	DrawEntities(e,p);	
	ScreenTexture.update(ScreenImage);
}

void Map::DrawMap(sf::RenderWindow& window)
{
	window.draw(ScreenSprite);
	window.draw(gunsprite);
}

void Map::DrawBackGround(PlayerParams& p)
{
	for (int x = 0; x < ScreenWidth; ++x)
	{
		//compute the agle of individual rays
		float RayAngle = (p.ViewDirection - p.FOVRad / 2.0f) + ((float)x / (float)ScreenWidth) * p.FOVRad;	

		for (float i = 0; i < MaxDepth; i += 0.06f)			//trace the ray position at each angle
		{
			int ray_posx = p.player_posx + i * cosf(RayAngle);
			int ray_posy = p.player_posy + i * sinf(RayAngle);

			//check if raytrack goes outside of the map
			if (ray_posx <0 || ray_posx > MapWidth || ray_posy < 0 || ray_posy > MapHeight)
			{
				p.DistanceToWall = MaxDepth;
				break;
			}

			//hits a wall
			else if (map[ray_posx + ray_posy * MapWidth] == '#')
			{
				p.DistanceToWall = i;

				float MidBlockX = (float)ray_posx + 0.5f;
				float MidBlockY = (float)ray_posy + 0.5f;

				float RayCollideX = p.player_posx + p.DistanceToWall * cosf(RayAngle);	//get ray collisino point
				float RayCollideY = p.player_posy + p.DistanceToWall * sinf(RayAngle);	//get ray collision point

				float Angle = atan2f((RayCollideY - MidBlockY) , (RayCollideX - MidBlockX));		//get the angle
				
				if (Angle >= -3.14159 * 0.25 && Angle < 3.14159 * 0.25)		//left 
				{
					SampleWallTextureX = RayCollideY - (float)ray_posy;
				}

				if (Angle >= 3.14159 * 0.25 && Angle < 3.14159 * 0.75)	//top
				{
					SampleWallTextureX = RayCollideX - (float)ray_posx;
				}

				if (Angle < -3.14159 * 0.25 && Angle >= -3.14159 * 0.75)	//bottom
				{
					SampleWallTextureX = RayCollideX - (float)ray_posx;
				}

				if (Angle < -3.14159 * 0.75 || Angle >= 3.14159 * 0.75)	//right
				{
					SampleWallTextureX = RayCollideY - (float)ray_posy;
				}

				break;
			}
		}

		CeilingStart = (int)( (float)(ScreenHeight * 0.5) - ((float)(ScreenHeight) /( p.DistanceToWall * cosf(RayAngle - p.ViewDirection))) );
		FloorStart = ScreenHeight - CeilingStart;
		ZBuffer[x] = p.DistanceToWall;	//update buffer

		for (int y = 0; y < ScreenHeight; ++y)			//for each height
		{
			if (y <= CeilingStart)
			{
				ScreenImage.setPixel(x, y, sf::Color::Black);
			}

			else if (y > CeilingStart && y <= FloorStart)
			{
				SampleWallTextureY = (float)(y - CeilingStart) / (float)(FloorStart - CeilingStart);
				ScreenImage.setPixel(x, y, wallimage.getPixel((int)(SampleWallTextureX * (wallimagewidth - 1)), (int)(SampleWallTextureY * (wallimageheight - 1))));
			}

			else if (y > FloorStart){
				ScreenImage.setPixel(x, y, sf::Color(0, 255, 0));
			}
		}
	}
}

void Map::DrawEntities(std::vector<std::shared_ptr<EntityParams>>& e,PlayerParams& p)
{
	//sprite angle should be relative to the player
	for (auto& entity : e) {

		float SpriteDirection = atan2f((entity->entity_posy- p.player_posy), (entity->entity_posx - p.player_posx));		//angle of sprite

		while (SpriteDirection - p.ViewDirection < -3.14159f)
		{
			SpriteDirection += 2.0f * 3.14159;
		}

		while (SpriteDirection - p.ViewDirection > 3.14159f)
		{
			SpriteDirection -= 2.0f * 3.14159;
		}

		float dx = entity->entity_posx - p.player_posx;
		float dy = entity->entity_posy - p.player_posy;
		float DistanceFromPlayer = sqrt((dx * dx) + (dy * dy));

		//draw sprite only if it is within FOV range and between -.4 and max distance
		if (std::fabs(SpriteDirection - p.ViewDirection) < p.FOVRad * 0.5f && DistanceFromPlayer > 2.0f && DistanceFromPlayer < MaxDepth)
		{
			float EntityHeightStart = (ScreenHeight * 0.5) - ScreenHeight / (DistanceFromPlayer * cosf(SpriteDirection - p.ViewDirection));
			float EntityFloor = ScreenHeight - EntityHeightStart;
			float EntityHeight = EntityFloor - EntityHeightStart;
			float EntityWidth = EntityHeight * (entity->enemyimage.getSize().x / entity->enemyimage.getSize().y);
			float EntityCenter = (0.5f * ((SpriteDirection - p.ViewDirection) / (p.FOVRad * 0.5)) + 0.5f) * (float)ScreenWidth;

			for (int x = 0; x < (int)EntityWidth; ++x)
			{
				for (int y = 0; y < (int)EntityHeight; ++y)
				{
					SampleEnemyTextureX = ((float)x / EntityWidth) * (float)entity->enemyimage.getSize().x;
					SampleEnemyTextureY = ((float)y / EntityHeight) * (float)entity->enemyimage.getSize().y;

					int xplot = (int)(EntityCenter + x - (EntityWidth / 2.0f));
					int yplot = y + EntityHeightStart;

					if (xplot > 0 && xplot < ScreenWidth)
					{
						if (ZBuffer[xplot] >= DistanceFromPlayer)
						{
							const auto& getpixel = entity->enemyimage.getPixel((unsigned int)SampleEnemyTextureX, (unsigned int)SampleEnemyTextureY);
							if (getpixel.a != 0)			//plot non-transparent pixels
							{
								ScreenImage.setPixel(xplot, yplot, getpixel);
								ZBuffer[xplot] = DistanceFromPlayer;
							
							}
						}
					}
				}
			}
		}
	}

	

	
	
}

void Map::LoadMapFromFile()
{
	ScreenImage.create(ScreenWidth, ScreenHeight, sf::Color::Black);
	ScreenTexture.loadFromImage(ScreenImage);
	ScreenSprite.setTexture(ScreenTexture);
	std::wifstream loadmap("map.txt");

	if (!loadmap)
	{
		throw std::runtime_error("map loading failed.");
	}

	else {
		std::wstring readmap;
		while (std::getline(loadmap, readmap)) {
			map += readmap;
			MapHeight += 1;
			MapWidth = readmap.length();
			readmap.clear();
		}
	}

	MaxDepth = std::max(MapHeight, MapWidth);
}

bool Map::CollideWall(Player& p)
{ 
	//if player hits the wall
	if (map[(int)p.GetPlayerParams().player_posx + MapWidth * (int)p.GetPlayerParams().player_posy] == '#')
	{
		return true;
	}

	return false;
}

std::wstring Map::GetMap() const
{
	return map;
}

int Map::GetMapWidth() const
{
	return MapWidth;
}

int Map::GetMapHeight() const
{
	return MapHeight;
}

