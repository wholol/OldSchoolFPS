#include "Enemy.h"
#include "Map.h"
#include <algorithm>
#include <iostream>
#include <thread>

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
			entitypositions.insert(std::make_pair((int)posx , (int)posy));
		}
		else {
			throw std::runtime_error("another entity exists in that position.");
		}
	}
	//get hthe image of the particular entity
	sf::Image& EntityImage = ImageManager::GetInstanace().getImage(image);	

	auto NewEntity = std::make_shared<EntityParams>( posx , posy ,EntityImage,health.value_or(INT_MAX),stationary );

	//insert into entity map
	if (stationary) {
		entitymap.insert(std::make_pair((int)posx + (int)posy * map.GetMapWidth(), NewEntity));
	}
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
		//if object is dynamic/moving
		if (!e->stationary) {

			//case 1: entity hits a wall.
			if ((int)e->entity_posx + map.GetMapWidth() * (int)e->entity_posy < map.GetMapWidth() * map.GetMapHeight()) {
				if (map.GetMap()[(int)e->entity_posx + map.GetMapWidth() * (int)e->entity_posy] == '#')
				{
					e->deleteEntitiy = true;	//delete moving entity
				}
			}

			if ((int)e->entity_posx + map.GetMapWidth() * (int)e->entity_posy > map.GetMapWidth() * map.GetMapHeight()) {
				e->deleteEntitiy = true;
			}

			//case 2: hits non living/stationary entity
			for (auto it = entitypositions.begin(); it != entitypositions.end(); ++it)
			{
				float posx = it->first;		
				float posy = it->second;

				//get distanc between lviing and non living entity
				float distance = sqrt((posx - e->entity_posx) * (posx - e->entity_posx) + (posy - e->entity_posy) * (posy - e->entity_posy));

				//if distance is lesser than 1.0f, moving entity hits non moving entity
				if (std::fabs(distance) < 1.0f)
				{
					std::cout << "hit" << std::endl;
					e->deleteEntitiy = true;	//delete moving entity

					//find the entity in which the moving entity hits
					auto findEntity = entitymap.find((int)posx + map.GetMapWidth() * (int)posy);

					if (findEntity != entitymap.end())
					{
						if (findEntity->second->health != INT_MAX)
						{
							findEntity->second->health -= 10;
						}

						//if entity runs of out health
						if (findEntity->second->health == 0)
						{
							//erase the entity from the map.
							entitymap.erase((int)posx + map.GetMapWidth() * (int)posy);

							findEntity->second->deleteEntitiy = true;
						}
					}
				}
			}
		}
	}

	//deletes entity from the vector of entities
	Entities.erase(std::remove_if(Entities.begin(), Entities.end(), [](std::shared_ptr<EntityParams>& params) { return params->deleteEntitiy; }), Entities.end());
}
