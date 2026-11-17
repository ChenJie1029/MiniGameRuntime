#pragma once
#include <filesystem>

#include "Core/World.h"
#include "Common/Entity.h"

struct Scene {
	World world;
	Entity player = INVALID_ENTITY; // 	INVALID_ENTITY：项目已有的常量，值为0，表示尚未指定有效实体   让Scene记住“哪个实体是玩家”

	Entity wall = INVALID_ENTITY;

	Entity collectible = INVALID_ENTITY;
	int score = 0;

	void Initialize(); // Scene提供一个名为Initialize的操作
	void DestroyEntity(Entity entity);
	void Reset();

	bool Save(const std::filesystem::path& path); // 存档文件的路径
	bool Load(const std::filesystem::path& path);
};