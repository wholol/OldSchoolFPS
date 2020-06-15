#include "Enemy.h"
#include "Map.h"

Enemy::Enemy(const Map& map)
	:map(map)
{

	if (!enemy.loadFromFile("enemy.png")) {
		throw std::runtime_error("enemy image failed to load.");
	}

}

void Enemy::addEnemy(int enemy_posx, int enemy_posy)
{
	if (map.GetMap()[enemy_posx + enemy_posy * map.GetMapWidth()] == '#')
	{
		throw std::runtime_error("enemy position already has a wall.");
	}

	Enemies.push_back({(float)enemy_posx , (float)enemy_posy ,enemy});
}

std::vector<EnemyParams> Enemy::GetEnemies() const
{
	return Enemies;
}
