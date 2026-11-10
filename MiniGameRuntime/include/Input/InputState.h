#pragma once

#include "Math/Vec2.h"

struct InputState {
	Vec2 playerDirection{};
	Vec2 cameraDirection{};
};

InputState ReadInputState();
