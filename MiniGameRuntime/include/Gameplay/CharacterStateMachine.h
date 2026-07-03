#pragma once

#include <memory>

#include "CharacterState.h"
#include "Common/Entity.h"

class CharacterStateMachine {
public:
	CharacterStateMachine() = default;

	void Update(Entity entity, float deltaTime);

	void ChangeState(Entity entity, std::shared_ptr<CharacterState> newState);

	std::shared_ptr<CharacterState> GetCurrentState() const { return m_currentState; }

private:
	std::shared_ptr<CharacterState> m_currentState = nullptr; // 当前执掌大权的状态
};