#include <iostream>

#include "Gameplay/CharacterStateMachine.h"

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