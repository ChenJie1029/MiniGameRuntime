#pragma once
#include <unordered_map>

#include "Common/Entity.h"
#include "Core/EntityManager.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/AABBComponent.h"

class World {
public:
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	// 位置组件接口
	void AddTransform(Entity entity, const TransformComponent& component); //创建实体并带有TransformComponent属性
	TransformComponent& GetTransform(Entity entity); //获取指定实体的位置组件（返回引用，允许外部直接修改它）
	bool HasTransform(Entity entity) const; //辅助安全检查：看看某个实体有没有位置属性

	// 速度组件接口
	void AddVelocity(Entity entity, const VelocityComponent& component);
	VelocityComponent& GetVelocity(Entity entity);
	bool HasVelocity(Entity entity) const;

	// 碰撞组件接口
	void AddAABB(Entity entity, const AABBComponent& component);
	AABBComponent& GetAABB(Entity entity);
	bool HasAABB(Entity entity) const;

	void Update(float deltaTime); // 世界更新心跳
	// 每过去一帧，main 函数里的主循环就会调用一次 world.Update()，告诉世界：“时间又前进了，请更新你内部所有实体的状态！”

private:
	EntityManager m_entityManager;

	// 位置组件大账本：Entity ID->TransformComponent
	std::unordered_map<Entity, TransformComponent> m_transformComponents;

	// 账本 2：Entity ID -> 速度组件
	std::unordered_map<Entity, VelocityComponent> m_velocityComponents;

	// 3. 碰撞组件大账本：Entity ID -> AABBComponent
	std::unordered_map<Entity, AABBComponent> m_aabbComponents;

	// 友元 特权通行证：允许 CollisionSystem 能够直接读取私有账本，进行批量物理计算
	friend class CollisionSystem;
};