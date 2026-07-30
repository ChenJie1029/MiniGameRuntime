#include "include/Core/Timer.h"

namespace Hazel {
	Timer::Timer() {
		Reset();
	}

	void Timer::Reset() {
		m_startTime = Clock::now(); // // 获取当前硬件芯片的高精度时间戳作为起点
	}

	float Timer::ElapsedSeconds() const {
		// 当前时间点 - 起始时间点 = 时间差
		auto currentTime = Clock::now();
		std::chrono::duration<float> elapsed = currentTime - m_startTime;
		return elapsed.count(); // 转换为秒（以 float 形式返回）
	}

	float Timer::ElapsedMilliseconds() const {
		// 1 秒 = 1000 毫秒
		return ElapsedSeconds() * 1000.0f;
	}
}