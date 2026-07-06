#pragma once

#include "Common/Entity.h"

// 最小事件：电报本身，只存数据
struct CollisionEvent {
	Entity a;
	Entity b;
};

// 听众接口：谁想听电报，谁就得继承这个接口
class ICollisionListener {
public:
	virtual ~ICollisionListener() = default;

	// 只要有碰撞，电报局就会强行调用这个函数
	virtual void OnCollision(const CollisionEvent& event) = 0;
};
