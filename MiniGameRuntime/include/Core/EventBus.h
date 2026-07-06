#pragma once

#include <vector>

#include "Event.h"

class EventBus {
public:
	// 1. 订阅窗口：听众把自己的指针（胶囊）交上来，登记进名单
	static void Subscribe(ICollisionListener* listener);

	// 2. 广播窗口：物理系统扔进一封电报，电报局负责通知所有人
	static void Publish(const CollisionEvent& event);

	// 3. 清空窗口：游戏结束时清空名单
	static void Clear();

private:
	// 内部的小本子：记录了所有正在听广播的听众
	static std::vector<ICollisionListener*> m_listeners;
};


