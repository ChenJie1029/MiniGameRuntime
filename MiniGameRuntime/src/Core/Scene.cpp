#include <vector>

#include "Core/Scene.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/AABBComponent.h"
#include "Math/Vec2.h"

void Scene::Initialize() {
	player = world.CreateEntity();
	world.AddTransform(player, TransformComponent{Vec2{-0.4f, 0.0f}});
	world.AddAABB(player, AABBComponent{ 0.2f, 0.2f });
	world.AddVelocity(player, VelocityComponent{ Vec2{0.0f, 0.0f} });

	Entity wall = world.CreateEntity();
	world.AddTransform(wall, TransformComponent{ Vec2{0.5f, 0.0f} });
	world.AddAABB(wall, AABBComponent{ 0.2f, 0.6f });

	renderEntities = { player, wall };
}

void Scene::DestroyEntity(Entity entity) {
	if (entity == INVALID_ENTITY) {
		return;
	}

	world.DestroyEntity(entity);
	std::erase(renderEntities, entity); // 移除绘制名单中的编号

	if (player == entity) { // 如果被销毁的是玩家，就把Scene记录的玩家ID改回无效值
		player = INVALID_ENTITY;
	}
}