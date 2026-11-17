#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include <locale>
#include <system_error>

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

	wall = world.CreateEntity();
	world.AddTransform(wall, TransformComponent{ Vec2{0.5f, 0.0f} });
	world.AddAABB(wall, AABBComponent{ 0.2f, 0.6f });
	world.AddSprite(wall, SpriteComponent{ Vec2{0.2f, 0.6f} });

	Entity movingMarker = world.CreateEntity();
	world.AddTransform(movingMarker, TransformComponent{ Vec2{-0.65f, 0.45f} });
	world.AddVelocity(movingMarker, VelocityComponent{ Vec2{0.12f, 0.0f} });
	world.AddSprite(movingMarker, SpriteComponent{ Vec2{0.14f, 0.14f} });

	collectible = world.CreateEntity();
	world.AddTransform(collectible, TransformComponent{ Vec2{-0.4f, -0.5f} });
	world.AddAABB(collectible, AABBComponent{ 0.1f, 0.1f });
	world.AddSprite(collectible, SpriteComponent{ Vec2{0.1f, 0.1f} });
}

void Scene::DestroyEntity(Entity entity) {
	if (entity == INVALID_ENTITY) {
		return;
	}

	world.DestroyEntity(entity);

	if (player == entity) { // 如果被销毁的是玩家，就把Scene记录的玩家ID改回无效值
		player = INVALID_ENTITY;
	}

	if (wall == entity) {
		wall = INVALID_ENTITY;
	}

	if (collectible == entity) {
		collectible = INVALID_ENTITY;
	}
}

void Scene::Reset() {
	world.Clear();
	player = INVALID_ENTITY;
	wall = INVALID_ENTITY;
	collectible = INVALID_ENTITY;
	score = 0;
	Initialize();
}

bool Scene::Save(const std::filesystem::path& path) {
	if (!world.HasTransform(player)) {
		return false;
	}

	const Vec2& position = world.GetTransform(player).position;
	if (!std::isfinite(position.x) || !std::isfinite(position.y) || (score != 0 && score != 1)) {
		return false;
	}

	std::filesystem::path tempPath = path;
	tempPath += ".tmp";
	std::ofstream file(tempPath);
	if (!file) {
		return false;
	}

	file.imbue(std::locale::classic());
	file << std::setprecision(std::numeric_limits<float>::max_digits10)
		<< '[' << position.x << ", " << position.y << ", " << score << "]\n";
	file.close();

	std::error_code error;
	if (file) {
		std::filesystem::rename(tempPath, path, error);
		if (!error) {
			return true;
		}
	}

	std::filesystem::remove(tempPath, error);
	return false;
}

bool Scene::Load(const std::filesystem::path& path) {
	std::ifstream file(path);
	file.imbue(std::locale::classic());

	Vec2 position{};
	int loadedScore = 0;
	char openBracket{}, firstComma{}, secondComma{}, closeBracket{};
	if (!(file >> openBracket >> position.x >> firstComma
		>> position.y >> secondComma >> loadedScore >> closeBracket)) {
		return false;
	}

	file >> std::ws;
	const bool validFormat = openBracket == '[' && firstComma == ',' &&
		secondComma == ',' && closeBracket == ']' &&
		file.eof() && !file.bad();
	const bool validPosition = std::isfinite(position.x) && std::isfinite(position.y);
	const bool validScore = loadedScore == 0 || loadedScore == 1;
	if (!validFormat || !validPosition || !validScore) {
		return false;
	}

	Reset();
	world.GetTransform(player).position = position;
	score = loadedScore;
	if (score == 1) {
		DestroyEntity(collectible);
	}
	return true;
}