#pragma once

#include <memory>

#include "CharacterState.h"
#include "Common/Entity.h"
#include "Core/Event.h"

class CharacterStateMachine : public ICollisionListener {
public:
	CharacterStateMachine() = default;

	void Update(Entity entity, float deltaTime);

	void ChangeState(Entity entity, std::shared_ptr<CharacterState> newState);

	std::shared_ptr<CharacterState> GetCurrentState() const { return m_currentState; }

	void OnCollision(const CollisionEvent& event) override;

private:
	std::shared_ptr<CharacterState> m_currentState = nullptr; // 当前执掌大权的状态
};