#include <vector>

#include "Core/Scene.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/AABBComponent.h"
#include "Math/Vec2.h"
#include "Components/SpriteComponent.h"

void Scene::Initialize() {
	player = world.CreateEntity();
	world.AddTransform(player, TransformComponent{Vec2{-0.4f, 0.0f}});
	world.AddAABB(player, AABBComponent{ 0.2f, 0.2f });
	world.AddVelocity(player, VelocityComponent{ Vec2{0.0f, 0.0f} });
	world.AddSprite(player, SpriteComponent{ Vec2{0.2f, 0.2f} });

	Entity wall = world.CreateEntity();
	world.AddTransform(wall, TransformComponent{ Vec2{0.5f, 0.0f} });
	world.AddAABB(wall, AABBComponent{ 0.2f, 0.6f });
	world.AddSprite(wall, SpriteComponent{ Vec2{0.2f, 0.6f} });

	Entity movingMarker = world.CreateEntity();
	world.AddTransform(movingMarker, TransformComponent{ Vec2{-0.65f, 0.45f} });
	world.AddVelocity(movingMarker, VelocityComponent{ Vec2{0.12f, 0.0f} });
	world.AddSprite(movingMarker, SpriteComponent{ Vec2{0.14f, 0.14f} });
}

void Scene::DestroyEntity(Entity entity) {
	if (entity == INVALID_ENTITY) {
		return;
	}

	world.DestroyEntity(entity);

	if (player == entity) { // 如果被销毁的是玩家，就把Scene记录的玩家ID改回无效值
		player = INVALID_ENTITY;
	}
}

void Scene::Reset() {
	world.Clear();
	player = INVALID_ENTITY;
	Initialize();
}