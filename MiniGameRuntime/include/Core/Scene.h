#pragma once

#include <vector>

#include "Core/World.h"
#include "Common/Entity.h"

struct Scene {
	World world;
	Entity player = INVALID_ENTITY; // 	INVALID_ENTITY：项目已有的常量，值为0，表示尚未指定有效实体   让Scene记住“哪个实体是玩家”
	std::vector<Entity> renderEntities;

	void Initialize(); // Scene提供一个名为Initialize的操作
};