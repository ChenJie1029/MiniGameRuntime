#pragma once

#include <iostream>
#include "Core/ObjectPool.h"

namespace Hazel {
	// 假设这是一个标准的 Entity 数据组件

	class BenchmarkEntity : public IPoolable {
	public:
		uint32_t id = 0;
		float transform[16] = { 0 };
		float health = 100.0f;
		bool isActive = false;

		BenchmarkEntity() = default;

		// 实现 IPoolable 协议
		void Init() override {
			health = 100.0f;
			isActive = true;
		}

		void Reset() override {
			health = 0.0f;
			isActive = false;
		}
	};
}
