#pragma once
#include "Common/Entity.h"

class CharacterStateMachine; // 前置声明

// 状态基类
class CharacterState {
public:
	virtual ~CharacterState() = default;

	// 当角色切换进我这个状态的那一瞬间（比如刚落地进入站立状态）
	virtual void Enter(Entity entity, CharacterStateMachine& fsm) = 0;

	// 只要保持在这个状态，每一帧都要执行的逻辑（比如在移动状态下持续加位移）
	virtual void Update(Entity entity, float deltaTime, CharacterStateMachine& fsm) = 0;

	// 当准备离开这个状态的那一瞬间（比如离开移动状态，把跑步特效关掉）
	virtual void Exit(Entity entity, CharacterStateMachine& fsm) = 0;
};
