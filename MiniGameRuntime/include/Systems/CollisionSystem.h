#pragma once

#include "Core/World.h"

class CollisionSystem {
public:
	// 系统的核心职责：接收整个世界，在内部完成所有实体的碰撞巡检
	void Update(World& world);
};