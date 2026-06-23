#include "Components/TransformComponent.h"

TransformComponent::TransformComponent()
	:position(0.0f, 0.0f), 
	 rotation(0.0f), 
	 scale(1.0f, 1.0f){}


TransformComponent::TransformComponent(const Vec2& positionValue)
	:position(positionValue),
	 rotation(0.0f),
	 scale(1.0f, 1.0f){}