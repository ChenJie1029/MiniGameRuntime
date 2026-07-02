#include <iostream>
#include <vector>
#include "Systems/CollisionSystem.h"
#include "Physics/PhysicsUtils.h"

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

			const Vec2& posA = world.GetTransform(entityA).position;
			const Vec2& halfA = world.GetAABB(entityA).halfExtents;

			const Vec2& posB = world.GetTransform(entityB).position;
			const Vec2& halfB = world.GetAABB(entityB).halfExtents;

			if (PhysicsUtils::CheckAABBCollision(posA, halfA, posB, halfB)) {
				std::cout << "[CollisionSystem] 💥 Detected! Entity [" << entityA
					<< "] collided with Entity [" << entityB << "]" << std::endl;
			}
		}
	}
}