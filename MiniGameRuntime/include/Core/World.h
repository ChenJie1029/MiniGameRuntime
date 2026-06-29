#pragma once
#include "Common/Entity.h"
#include "Core/EntityManager.h"

class World {
public:
	Entity CreateEntity();
	void DestoryEntity(Entity entity);

private:
	EntityManager m_entityManager;
};