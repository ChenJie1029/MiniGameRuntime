#pragma once

#include "Math/Vec2.h"

class PhysicsUtils {
public:
	static bool CheckAABBCollision( // 两个盒子需要：各自的中心点位置（pos） + 各自的半宽高（halfExtents）
		const Vec2& posA, const Vec2& halfA,
		const Vec2& posB, const Vec2& halfB) {

		// 1. 算出 A 盒子的四个边界
		float minA_x = posA.x - halfA.x;
		float maxA_x = posA.x + halfA.x;
		float minA_y = posA.y - halfA.y;
		float maxA_y = posA.y + halfA.y;

		// 2. 算出 B 盒子的四个边界
		float minB_x = posB.x - halfB.x;
		float maxB_x = posB.x + halfB.x;
		float minB_y = posB.y - halfB.y;
		float maxB_y = posB.y + halfB.y;

		// 判断是否错开
		if (maxA_x < minB_x || minA_x > maxB_x) return false;
		if (maxA_y < minB_y || minA_y > maxB_y) return false;

		return true;
	}
};
