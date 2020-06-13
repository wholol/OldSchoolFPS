#include "Map.h"
#include <fstream>
Map::Map(int ScreenWidth, int ScreenHeight)
	:ScreenWidth(ScreenWidth), ScreenHeight(ScreenHeight)
{
	image.create(ScreenWidth, ScreenHeight, sf::Color::Black);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	
	if (!guntexture.loadFromFile("gun.png")) {
		throw std::runtime_error("gun image loading failed.");
	}

	gunsprite.setTexture(guntexture);
	gunsprite.setPosition(sf::Vector2f((ScreenWidth - 57)/2, (ScreenHeight - 72)));
	
	if (!wallimage.loadFromFile("bricks.png"))
	{
		throw std::runtime_error("wall image loading failed.");
	}

	wallimagewidth = wallimage.getSize().x;
	wallimageheight = wallimage.getSize().y;
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


void Map::UpdateMap(Player& player)
{
	PlayerParams p = player.GetPlayerParams();
	ComputePlayerRayCast(p);
	texture.update(image);
}

void Map::DrawMap(sf::RenderWindow & window)
{
	window.draw(sprite);
	window.draw(gunsprite);
}

void Map::ComputePlayerRayCast(PlayerParams& p)
{

	for (int x = 0; x < ScreenWidth; ++x)
	{
		//compute the agle of individual rays
		float RayAngle = (p.ViewDirection - p.FOVRad / 2.0f) + ((float)x / (float)ScreenWidth) * p.FOVRad;

		for (float i = 0; i < MaxDepth; i += 0.1f)			//trace the ray position at each angle
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

		for (int y = 0; y < ScreenHeight; ++y)			//for each height
		{
			if (y <= CeilingStart)
			{
				image.setPixel(x, y, sf::Color::Black);
			}

			else if (y > CeilingStart && y <= FloorStart)
			{
				SampleWallTextureY = (float)(y - CeilingStart) / (float)(FloorStart - CeilingStart);
				image.setPixel(x, y, wallimage.getPixel(SampleWallTextureX * (wallimagewidth - 1) , SampleWallTextureY * (wallimageheight - 1)));
			}

			else if (y > FloorStart){

				float Ratio = (float)(y - (float)ScreenHeight * 0.5) / (float)(ScreenHeight * 0.5);
				if (Ratio < 0.25)
				{
					image.setPixel(x, y, sf::Color(0, 255, 0));
				}

				else if (Ratio < 0.5)
				{
					image.setPixel(x, y, sf::Color(0, 200, 0));
				}

				else if (Ratio < 0.75)
				{
					image.setPixel(x, y, sf::Color(0, 169, 0));
				}

				else {
					image.setPixel(x, y, sf::Color(0, 109, 0));
				}
			}
		}
	}
}

bool Map::HitWall(Player& p)
{ 
	//if player hits the wall
	if (map[(int)p.GetPlayerParams().player_posx + MapWidth * (int)p.GetPlayerParams().player_posy] == '#')
	{
		return true;
	}
	return false;
}

