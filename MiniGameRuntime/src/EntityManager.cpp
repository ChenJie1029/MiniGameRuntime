#include "Core/EntityManager.h"
#include <iostream>
#include <algorithm>

EntityManager::EntityManager() : m_nextEntityID(1){}

//创建实体
Entity EntityManager::CreateEntity() {
	Entity newEntity = m_nextEntityID;
	m_nextEntityID++;

	m_livingEntities.push_back(newEntity);
	return newEntity;
}

//查询实体是否存活
bool EntityManager::IsAlive(Entity entity) const {
	// 非法的 ID 绝对不可能是活着的
	if (entity == INVALID_ENTITY) {
		return false;
	}

	auto it = find(m_livingEntities.begin(), m_livingEntities.end(), entity);

	return it != m_livingEntities.end();
}

//删除指定的实体
void EntityManager::DestroyEntity(Entity entity) {
	// 不能删除无效的 Entity
	if (entity == INVALID_ENTITY) {
		std::cout << "[Warning] Attempted to destroy INVALID_ENTITY. Ignored." << std::endl;
		return;
	}
	// 不能重复删除（如果当前不存活，直接拦截
	if (!IsAlive(entity)) {
		std::cout << "[Warning] Attempted to destroy a non-existent or already destroyed Entity:" << entity << ". Ignored." << std::endl;
		return;
	}

	// 经典做法：Erase-Remove 惯用法
	// remove 会把要删的元素移到末尾，并返回一个指向末尾待删区开头的迭代器
	// 然后 erase 把这个待删区彻底拔除
	m_livingEntities.erase(
		remove(m_livingEntities.begin(), m_livingEntities.end(), entity),
		m_livingEntities.end()
	);
}

//查询当前存活的实体数量
size_t EntityManager::GetEntityCount() const {
	return m_livingEntities.size();
}

