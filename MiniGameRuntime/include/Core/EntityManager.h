#pragma once

#include <vector>
#include "Common/Entity.h"

class EntityManager {
public:
	EntityManager(); //构造函数

	Entity CreateEntity(); //创建单个实体，返回它拿到的新 ID

	bool IsAlive(Entity entity) const; //查询实体是否存活

	void DestroyEntity(Entity entity); //删除指定的实体

	size_t GetEntityCount() const; //查询当前存活的实体数量

private:
	Entity m_nextEntityID; //记录下一个可用的 ID
	std::vector<Entity> m_livingEntities; //存放当前所有存活实体的名册
};