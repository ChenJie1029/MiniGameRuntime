#pragma once

#include <vector>
#include <cmath>
#include "Core/Timer.h"

namespace Hazel {
	// 2D 轴对齐包围盒 (AABB): Min(x1, y1), Max(x2, y2)
	// 采用 SoA 内存连续排布，内存 100% 对齐 Cache Line
	class CollisionSystemDOD {
	public:
		// 所有碰撞盒的最小/最大坐标扁平化存储在连续内存中
		std::vector<float> minX;
		std::vector<float> minY;
		std::vector<float> maxX;
		std::vector<float> maxY;

		size_t count = 0;

		// 预分配内存并初始化随机 AABB 包围盒
		void Reserve(size_t capacity) {
			count = capacity;

			minX.resize(capacity);
			minY.resize(capacity);
			maxX.resize(capacity);
			maxY.resize(capacity);

			// 填充测试数据：生成分布在空间中的不同包围盒
			for (size_t i = 0; i < capacity; ++i) {
				float x = static_cast<float>(i % 100) * 2.0f;
				float y = static_cast<float>(i / 100) * 2.0f;

				// 长度为 1.5 的包围盒
				minX[i] = x;
				minY[i] = y;
				maxX[i] = x + 1.5f;
				maxY[i] = y + 1.5f;
			}
		}

		// AABB 相交测试（快速判定两个包围盒是否重叠）
		inline static bool IntersectAABB(float aMinX, float aMinY, float aMaxX, float aMaxY,
			float bMinX, float bMinY, float bMaxX, float bMaxY) {
			return(aMinX <= bMaxX && aMaxX >= bMinX) && (aMinY <= bMaxY && aMaxY >= bMinY);
		}

		// 核心碰撞检测循环：对 N 个物理对象进行两两检测
		size_t Update() {
			size_t collisionCount = 0;

			// 遍历所有可能的物体对 (i, j)
            #pragma omp parallel for reduction(+:collisionCount)
			for (int i = 0; i < static_cast<int>(count); ++i) {
				float aMinX = minX[i], aMinY = minY[i];
				float aMaxX = maxX[i], aMaxY = maxY[i];

				for (size_t j = i + 1; j < count; ++j) {
					if (IntersectAABB(aMinX, aMinY, aMaxX, aMaxY,
						minX[j], minY[j], maxX[j], maxY[j])) {
						collisionCount++;
					}
				}
			}

			return collisionCount;
		}

	};
}
