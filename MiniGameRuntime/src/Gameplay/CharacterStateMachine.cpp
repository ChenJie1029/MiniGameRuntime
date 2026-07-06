#include <iostream>

#include "Gameplay/CharacterStateMachine.h"
#include "Core/EventBus.h"

void CharacterStateMachine::Update(Entity entity, float deltaTime) {
	if (m_currentState) {
		m_currentState->Update(entity, deltaTime, *this);
	}
}

void CharacterStateMachine::ChangeState(Entity entity, std::shared_ptr<CharacterState> newState) {
	if (m_currentState) {
		m_currentState->Exit(entity, *this);
	}

	m_currentState = newState;

	if (m_currentState) {
		m_currentState->Enter(entity, *this);
	}
}

void CharacterStateMachine::OnCollision(const CollisionEvent& event) {
	Entity myEntityId = 1;

	// 检查：这封电报里的撞车惨案，主角是不是我？
	if (event.a == myEntityId || event.b == myEntityId) {
		std::cout << "[FSM] Received Collision Event! Oh no, it's ME! Changing state..." << std::endl;

		// 跨界指挥：无缝切换到【受伤状态】！
		// 这里的 this 指针不能加星号，因为 ChangeState 要的是大管家的引用，此时大管家在自己家里，直接传 *this 即可
		this->ChangeState(myEntityId, std::make_shared<HitState>());
	}
}