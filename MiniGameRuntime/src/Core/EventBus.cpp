#include "Core/EventBus.h"

// C++ 规定：类的静态成员变量，必须在 .cpp 里初始化一下
std::vector<ICollisionListener*> EventBus::m_listeners;

void EventBus::Subscribe(ICollisionListener* listener) {
	if (listener) {
		m_listeners.push_back(listener);
	}
}

void EventBus::Publish(const CollisionEvent& event) {
	for (ICollisionListener* listener : m_listeners) {
		listener->OnCollision(event);
	}
}

void EventBus::Clear() {
	m_listeners.clear();
}