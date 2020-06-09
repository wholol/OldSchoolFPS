#include "Map.h"

Map::Map(int MapWidth, int MapHeight, int ScreenWidth, int ScreenHeight)
	:MapWidth(MapWidth),MapHeight(MapHeight),ScreenWidth(ScreenWidth), ScreenHeight(ScreenHeight)
{
	image.create(ScreenWidth, ScreenHeight, sf::Color::Black);
	texture.loadFromImage(image);
	sprite.setTexture(texture);

	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";
}


void Map::UpdateMap(Player& player)
{
	PlayerParams p = player.GetPlayerParams();
	ComputePlayerRayCast(p);
}

void Map::DrawMap(sf::RenderWindow & window)
{
	window.draw(sprite);
}



void Map::ComputePlayerRayCast(PlayerParams& p)
{

	for (int x = 0; x < ScreenWidth; ++x)
	{
		//compute the agle of individual rays
		float RayAngle = (p.ViewDirection - p.FOVRad / 2.0f) + ((float)x / (float)ScreenWidth) * p.FOVRad;

		for (float i = 0; i < MaxDepth; i += 0.05f)			//trace the ray position at each angle
		{
			float ray_posx = p.player_posx + i * cos(RayAngle);
			float ray_posy = p.player_posy + i * sin(RayAngle);

			//check if raytrack goes outside of the map
			if (ray_posx <0 || ray_posx > MapWidth || ray_posy < 0 || ray_posy > MapHeight)
			{
				p.DistanceToWall = MaxDepth;
				break;
			}

			//hits a wall
			if (map[ray_posx + ray_posy * MapWidth] == '#')
			{
				p.DistanceToWall = i;
				break;
			}
		}


		CeilingStart = (int)((ScreenHeight * 0.5) - ((ScreenHeight) / p.DistanceToWall));
		FloorStart = (int)ScreenHeight - CeilingStart;

		for (int y = 0; y < ScreenHeight; ++y)			//for each height
		{
			if (y <= CeilingStart)
			{
				image.setPixel(x, y, sf::Color::Green);
				//draw ceiling
			}

			else if (y > CeilingStart && y <= FloorStart)
			{
				image.setPixel(x, y, sf::Color::Red);
			}

			else {
				image.setPixel(x, y, sf::Color::Cyan);
			}
		}
	}
}

