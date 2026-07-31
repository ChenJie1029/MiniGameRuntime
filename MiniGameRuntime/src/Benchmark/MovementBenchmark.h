#pragma once

#include <vector>
#include <immintrin.h>
#include "Core/Timer.h"

namespace Hazel {
	// 最优解架构：SoA (Structure of Arrays) 连续数据流

	class alignas(32) MovementSystemDOD {
	public:
		// 核心移动数据集中连续排布（紧凑物理内存）
		std::vector<float> posX;
		std::vector<float> posY;
		std::vector<float> posZ;

		std::vector<float> velX;
		std::vector<float> velY;
		std::vector<float> velZ;

		size_t count = 0;

		// 初始化预分配内存
		void Reserve(size_t capacity) {
			count = capacity;

			posX.resize(capacity, 0.0f);
			posY.resize(capacity, 0.0f);
			posZ.resize(capacity, 0.0f);

			velX.resize(capacity, 0.0f);
			velY.resize(capacity, 0.0f);
			velZ.resize(capacity, 0.0f);
		}

		// 核心更新逻辑：CPU 自动向量化 / 缓存预取极速循环
		void Update(float deltaTime) {
			// 数据在内存中 100% 连续，CPU Prefetcher 可以精准预测并预加载数据
            #pragma omp parallel for // 支持多线程并行加速
			for (size_t i = 0; i < count; ++i) {
				posX[i] += velX[i] * deltaTime;
				posY[i] += velY[i] * deltaTime;
				posZ[i] += velZ[i] * deltaTime;
			}
		}
	};

}