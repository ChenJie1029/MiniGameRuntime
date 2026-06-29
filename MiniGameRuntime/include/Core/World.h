#pragma once
#include <unordered_map>

#include "Common/Entity.h"
#include "Core/EntityManager.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"

class World {
public:
	Entity CreateEntity();
	void DestoryEntity(Entity entity);

	// 位置组件接口
	void AddTransform(Entity entity, const TransformComponent& component); //创建实体并带有TransformComponent属性
	TransformComponent& GetTransform(Entity entity); //获取指定实体的位置组件（返回引用，允许外部直接修改它）
	bool HasTransform(Entity entity) const; //辅助安全检查：看看某个实体有没有位置属性

	// 速度组件接口
	void AddVelocity(Entity entity, const VelocityComponent& component);
	VelocityComponent& GetVelocity(Entity entity);
	bool HasVelocity(Entity entity) const;

private:
	EntityManager m_entityManager;

	// 位置组件大账本：Entity ID->TransformComponent
	unordered_map<Entity, TransformComponent> m_transformComponents;

	// 账本 2：Entity ID -> 速度组件
	unordered_map<Entity, VelocityComponent> m_velocityComponents;
};