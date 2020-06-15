#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

struct EnemyParams
{
	EnemyParams(float enemy_posx, float enemy_posy, sf::Image& enemyimage)
		:enemy_posx(enemy_posx) , enemy_posy(enemy_posy) , enemyimage(enemyimage){}

	float enemy_posx, enemy_posy;			
	sf::Image &enemyimage;
	static constexpr int health = 100;
};

class Enemy
{
public:
	Enemy(const class Map& map);
	void addEnemy(int enemy_posx, int enemy_posy);
	std::vector<EnemyParams> GetEnemies() const;

private:
	std::vector<EnemyParams> Enemies;
	const Map& map;
	sf::Image enemy;

	
};