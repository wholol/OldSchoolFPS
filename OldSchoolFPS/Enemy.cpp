#include "Enemy.h"
#include "Map.h"
#include <algorithm>
#include <iostream>

Entity::Entity(const Map& map)
	:map(map) {}

void Entity::addEntity(float posx, float posy, bool stationary, const std::string& image , std::optional<int> health)
{
	//check position to see if it is a wall
	if (map.GetMap()[(int)posx + (int)posy * map.GetMapWidth()] == '#')
	{
		throw std::runtime_error("position already has a wall.");
	}
	
	//check for entities in that position (only for stationary entities)
	if (stationary) {
		if (entitypositions.find((int)posx + (int)posy * map.GetMapWidth()) == entitypositions.end())
		{
			entitypositions.insert((int)posx + (int)posy * map.GetMapWidth());
		}
		else {
			throw std::runtime_error("another entity exists in that position.");
		}
	}
	//get hthe image of the particular entity
	sf::Image& EntityImage = ImageManager::GetInstanace().getImage(image);	

	auto NewEntity = std::make_shared<EntityParams>( posx , posy ,EntityImage,health.value_or(INT_MAX),stationary );
	//insert into entity map
	entitymap.insert(std::make_pair( (int)posx + (int)posy * map.GetMapWidth(), NewEntity ));
	//construct entities
	//construct the health of non living objects to INT_MAX
	Entities.push_back(NewEntity);	 
}

std::vector<std::shared_ptr<EntityParams>> Entity::GetEntity() const
{
	return Entities;
}

void Entity::MoveEntities(Player& p)
{
	//move all entities (if non stationary)
	for (auto& e : Entities)
	{
		e->MoveEntity(p);
	}
}

void Entity::DeleteEntity()
{
	//deletes entity by checking for collisions with the wall or other entities.
	for (auto& e : Entities)
	{
		//case 1: entity hits a wall.
		if (map.GetMap()[(int)e->entity_posx + map.GetMapWidth() * (int)e->entity_posy] == '#')
		{
			e->deleteEntitiy = true;
		}

		//case 2:entity hits another entity (only applies to moving objects, i.e. bullet)
		if (!e->stationary){
			if (entitypositions.find((int)e->entity_posx + map.GetMapWidth() * (int)e->entity_posy) != entitypositions.end())
			{
				e->deleteEntitiy = true;

				//find the entity it collided with
				auto findEntity = entitymap.find((int)e->entity_posx + map.GetMapWidth() * (int)e->entity_posy);

				if (findEntity != entitymap.end())
				{
					//if the entity it collided with has health/lvinig thing
					if (findEntity->second->health != INT_MAX)
					{
						findEntity->second->health -= 10;
						std::cout << "i took 10 damage!" << std::endl;
					}

					//if entity runs of out health
					if (findEntity->second->health == 0)
					{
						//erase the entity from the map.
						entitymap.erase((int)e->entity_posx + map.GetMapWidth() * (int)e->entity_posy);

						findEntity->second->deleteEntitiy = true;
					}

				}
			}
		}
	}
	//deletes entity from the vector of entities
	Entities.erase(std::remove_if(Entities.begin(), Entities.end(), [](std::shared_ptr<EntityParams>& params) { return params->deleteEntitiy; }), Entities.end());
}
