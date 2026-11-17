#include <vector>
#include "Systems/CollisionSystem.h"
#include "Physics/PhysicsUtils.h"
#include "Core/EventBus.h"

bool CollisionSystem::Overlaps(World& world, Entity a, Entity b) const {
	if (!world.HasTransform(a) || !world.HasAABB(a) || !world.HasTransform(b) || !world.HasAABB(b)) { // 检查位置和碰撞盒
		return false;
	}

	return PhysicsUtils::CheckAABBCollision(world.GetTransform(a).position, world.GetAABB(a).halfExtents, world.GetTransform(b).position, world.GetAABB(b).halfExtents);
}

void CollisionSystem::Update(World& world) {
	// 自动化筛选：建立一个临时名单，把世界上所有同时有物理肉身(Transform)和碰撞体(AABB)的实体抓出
	std::vector<Entity> targets;

	for (auto& pair : world.m_transformComponents) {
		Entity e = pair.first;
		if (world.HasAABB(e)) {
			targets.push_back(e);
		}
	}

	// 双重循环：让名单里的实体进行两两配对（避免重复计算，用 j = i + 1）
	for (size_t i = 0; i < targets.size(); i++) {
		for (size_t j = i + 1; j < targets.size(); j++) {
			Entity entityA = targets[i];
			Entity entityB = targets[j];

			if (Overlaps(world, entityA, entityB)) {
				CollisionEvent newEvent;
				newEvent.a = entityA;
				newEvent.b = entityB;

				EventBus::Publish(newEvent);
			}
		}
	}
}