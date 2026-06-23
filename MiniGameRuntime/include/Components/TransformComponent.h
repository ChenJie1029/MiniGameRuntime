#pragma once
#include "Math/Vec2.h"

struct TransformComponent{
	Vec2 position; //自己坐标
	float rotation; //旋转角度
	Vec2 scale; //缩放比例

	TransformComponent();
	TransformComponent(const Vec2& positionValue);
};
