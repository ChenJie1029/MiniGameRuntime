#pragma once

#include "Math/Vec2.h"

struct InputState {
	Vec2 playerDirection{};
	Vec2 cameraDirection{};
};

InputState ReadInputState();

struct InputActions { // 每个布尔值表示本帧是否发生了这个动作
	bool quitRequested = false;
	bool resetRequested = false;
	bool deletePlayerRequested = false;
	bool pauseRequested = false;
};

InputActions ReadInputActions();
