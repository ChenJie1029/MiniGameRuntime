#pragma once

#include "Common/Entity.h"
#include "Core/World.h"
#include "Math/Vec2.h"

class PlayerControlSystem {
public:
	void Update(World& world, Entity player, const Vec2& direction, float moveSpeed);
};
