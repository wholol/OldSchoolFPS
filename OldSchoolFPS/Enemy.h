#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "ImageManager.h"
#include <unordered_map>
#include <optional>
#include "Player.h"

//parameter struct for an entity
struct EntityParams
{
	EntityParams(float entity_posx, float entity_posy, sf::Image& enemyimage, int health, bool stationary)
		:entity_posx(entity_posx), entity_posy(entity_posy), enemyimage(enemyimage),health(health), stationary(stationary) {}

	EntityParams& operator=(const EntityParams& other)	//needed for std::remove_if
	{
		deleteEntitiy = other.deleteEntitiy;
		entity_posx = other.entity_posx;
		entity_posy = other.entity_posy;
		stationary = other.stationary;
		enemyimage = other.enemyimage;
		health = other.health;
		return *this;
	}

	bool deleteEntitiy = false;
	float entity_posx;
	float entity_posy;
	bool stationary;
	sf::Image &enemyimage;
	int health;

	void MoveEntity(Player& p)		//moves the entity
	{
		auto playerparams = p.GetPlayerParams();
		if (!stationary) {
			entity_posx += cosf(playerparams.ViewDirection)*2.0f;
			entity_posy += sinf(playerparams.ViewDirection)*2.0f;
		}
	}
};


class Entity
{
public:
	Entity(const class Map& map);
	void addEntity(float entity_posx, float entity_posy, bool stationary, const std::string& image, std::optional<int> health = {});
	std::vector<std::shared_ptr<EntityParams>> GetEntity() const;
	void MoveEntities(Player& p);
	void DeleteEntity();

private:
	std::vector<std::shared_ptr<EntityParams>> Entities;		
	const Map& map;
	std::unordered_multimap<int,int> entitypositions;	//keep track of all stationary entitiy positions
	std::unordered_map<int, std::shared_ptr<EntityParams>> entitymap; //this map is used to determine which entity has been hit by the moving entities
};