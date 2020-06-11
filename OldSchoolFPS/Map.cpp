#include "Map.h"

Map::Map(int MapWidth, int MapHeight, int ScreenWidth, int ScreenHeight)
	:MapWidth(MapWidth),MapHeight(MapHeight),ScreenWidth(ScreenWidth), ScreenHeight(ScreenHeight)
{
	image.create(ScreenWidth, ScreenHeight, sf::Color::Black);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	guntexture.loadFromFile("gun.png");
	gunsprite.setTexture(guntexture);
	gunsprite.setPosition(sf::Vector2f((ScreenWidth - 57)/2, (ScreenHeight - 72)));

	map += L"###########################";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#......####...............#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#..................########";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.............####........#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#...####..................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"#.........................#";
	map += L"###########################";

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
			int ray_posx = p.player_posx + i * cos(RayAngle);
			int ray_posy = p.player_posy + i * sin(RayAngle);

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
				if (p.DistanceToWall < (float)MaxDepth / 4) {
					image.setPixel(x, y, sf::Color(255, 0, 0));
				}

				else if (p.DistanceToWall < (float)MaxDepth / 3) {
					image.setPixel(x, y, sf::Color(200, 0, 0));
				}

				else if (p.DistanceToWall < (float)MaxDepth / 2) {
					image.setPixel(x, y, sf::Color(150, 0, 0));
				}

				else if (p.DistanceToWall < (float)MaxDepth / 1.5) {
					image.setPixel(x, y, sf::Color(70, 0, 0));
				}
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

