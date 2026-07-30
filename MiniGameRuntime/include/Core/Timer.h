#pragma once

#include<chrono>

namespace Hazel {
	class Timer {
	public:
		Timer(); // 构造函数：创建 Timer 的瞬间会自动开始计时
		
		void Reset(); // 重新开始/重置计时器

		float ElapsedSeconds() const; // 获取从开始（或上一次 Reset）到当前所经过的时间（单位：秒 / s）

		float ElapsedMilliseconds() const; // 获取从开始（或上一次 Reset）到当前所经过的时间（单位：毫秒 / ms）

	private:
		// 使用 C++ 标准库的高精度时钟点类型
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::time_point<Clock>;

		TimePoint m_startTime;
	};
}
