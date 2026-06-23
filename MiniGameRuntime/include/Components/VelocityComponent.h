#pragma once
#include "Math/Vec2.h"

struct VelocityComponent {
	Vec2 velocity; //速度

	VelocityComponent();
	VelocityComponent(const Vec2& velocity);
};