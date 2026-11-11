#include "Systems/PlayerControlSystem.h"

#include "Components/VelocityComponent.h"

void PlayerControlSystem::Update(World& world, Entity player, const Vec2& direction, float moveSpeed) {
	if (!world.HasVelocity(player)) {
		return;
	}

	VelocityComponent& velocity = world.GetVelocity(player);
	velocity.velocity = direction * moveSpeed;
}