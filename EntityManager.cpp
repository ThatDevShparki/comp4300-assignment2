#include "EntityManager.h"

/* Constructors */

EntityManager::EntityManager() = default;


/* gameloop tick */


void EntityManager::update()
{
	// Add new entities
	for (auto& entity: m_entitiesToAdd)
	{
		m_entities.push_back(entity);
		m_entityMap[entity->tag()].push_back(entity);
	}
	m_entitiesToAdd.clear();

	// Remove dead entities
	removeDeadEntities(m_entities);
	for (auto& [tag, vec]: m_entityMap)
	{
		removeDeadEntities(vec);
	}
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	vec.erase(
		std::remove_if(
			vec.begin(), vec.end(), [](const auto& entity)
			{
				return !entity->isActive();
			}
		), vec.end());
}


/* Public Accessors */


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}


size_t EntityManager::entityCount()
{
	return m_entities.size();
}


