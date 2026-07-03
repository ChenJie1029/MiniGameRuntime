#pragma once

#include <memory>

#include "CharacterState.h"
#include "Common/Entity.h"

class CharacterStateMachine {
public:
	void Update(Entity entity, float deltaTime) {
		if (m_currentState) {
			m_currentState->Update(entity, deltaTime, *this);
		}
	}

	void ChangeState(Entity entity, std::shared_ptr<CharacterState> newState) {
		if (m_currentState) {
			m_currentState->Exit(entity, *this);
		}

		m_currentState = newState;

		if (m_currentState) {
			m_currentState->Enter(entity, *this);
		}
	}

private:
	std::shared_ptr<CharacterState> m_currentState = nullptr; // 当前执掌大权的状态
};