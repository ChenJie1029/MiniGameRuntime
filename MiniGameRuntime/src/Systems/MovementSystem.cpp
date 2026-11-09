#include "Systems/MovementSystem.h"
#include "Common/Entity.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"

void MovementSystem::Update(World& world, float deltaTime) {
	for (auto& entry : world.m_transformComponents) {
		const Entity entity = entry.first;

		if (world.HasVelocity(entity)) {
			TransformComponent& transform = entry.second;
			const VelocityComponent& velocityComponent = world.GetVelocity(entity);

			transform.position.x += velocityComponent.velocity.x * deltaTime;
			transform.position.y += velocityComponent.velocity.y * deltaTime;
		}
	}
}