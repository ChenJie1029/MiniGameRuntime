#include <stdexcept> //引入标准异常，用于处理非法查询

#include "Core/World.h"

Entity World::CreateEntity() {
	return m_entityManager.CreateEntity();
}

void World::DestoryEntity(Entity entity) {
	m_entityManager.DestroyEntity(entity);
	m_transformComponents.erase(entity); //如果这个实体被毁灭了，它在位置账本里的数据也必须一并注销
	m_velocityComponents.erase(entity);
}

void World::AddTransform(Entity entity, const TransformComponent& component) {
	if (entity == INVALID_ENTITY) return;

	m_transformComponents[entity] = component; //直接塞进哈希表。如果已经有了，这行代码也会直接覆盖，非常安全
}

TransformComponent& World::GetTransform(Entity entity) {
	auto it = m_transformComponents.find(entity);
	if (it == m_transformComponents.end()) {
		throw runtime_error("[ECS Error] Entity does not have a TransformComponent!");
	}

	return it->second;
}

// 检查是否拥有位置组件
bool World::HasTransform(Entity entity) const {
	return m_transformComponents.find(entity) != m_transformComponents.end();
}

void World::AddVelocity(Entity entity, const VelocityComponent& component) {
	if (entity == INVALID_ENTITY) return;

	m_velocityComponents[entity] = component;
}

VelocityComponent& World::GetVelocity(Entity entity) {
	auto it = m_velocityComponents.find(entity);
	if (it == m_velocityComponents.end()) {
		throw runtime_error("[ECS Error] Entity does not have a VelocityComponent!");
	}

	return it->second;
}

bool World::HasVelocity(Entity entity) const {
	return m_velocityComponents.find(entity) != m_velocityComponents.end();
}